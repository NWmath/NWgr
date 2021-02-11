//
// This file is part of the Graph Standard Library (aka BGL17 aka NWGraph)
// (c) 2020 Pacific Northwest National Laboratory
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//
static constexpr const char USAGE[] =
    R"(cc.exe: BGL17 connected components benchmark driver.
  Usage:
      cc.exe (-h | --help)
      cc.exe [-f FILE...] [-s FILE...] [--version ID...] [-n NUM] [--succession STR] [--relabel] [--clean] [--direction DIR] [-dvV] [--log FILE] [--log-header] [THREADS]...

  Options:
      -h, --help            show this screen
      --version ID          algorithm version to run [default: 0]
      -f FILE               asymmetric input file paths
      -s FILE               symmetric input file paths
      -n NUM                number of trials [default: 1]
      --relabel             relabel the graph or not
      -c, --clean           clean the graph or not
      --direction DIR       graph relabeling direction - ascending/descending [default: descending]
      --succession STR      successor/predecessor [default: successor]
      --log FILE            log times to a file
      --log-header          add a header to the log file
      -d, --debug           run in debug mode
      -v, --verify          verify results
      -V, --verbose         run in verbose mode
)";

#include "Log.hpp"
#include "adaptors/plain_range.hpp"
#include "algorithms/connected_components.hpp"
#include "common.hpp"
#include "containers/adjacency.hpp"
#include "containers/edge_list.hpp"
#include "util/atomic.hpp"
#include "util/traits.hpp"
#include <docopt.h>

#ifdef CL_SYCL_LANGUAGE_VERSION
#define cins dpstd
#else
#define cins tbb
#endif

using namespace nw::graph::bench;
using namespace nw::graph;
using namespace nw::util;

using vertex_id_type = default_vertex_id_type;

template <typename Vector>
static void link(vertex_id_type u, vertex_id_type v, Vector& comp) {
  vertex_id_type p1 = nw::graph::acquire(comp[u]);
  vertex_id_type p2 = comp[v];
  while (p1 != p2) {
    vertex_id_type high   = std::max(p1, p2);
    vertex_id_type low    = p1 + (p2 - high);
    vertex_id_type p_high = comp[high];

    if ((p_high == low) || (p_high == high && comp[high].compare_exchange_strong(high, low))) break;
    p1 = comp[p_high];
    p2 = comp[low];
  }
}

template <typename Execution, typename Graph, typename Vector>
static void compress(Execution exec, const Graph& g, Vector& comp) {
  std::for_each(exec, cins::counting_iterator(0ul), cins::counting_iterator(comp.size()), [&](auto n) {
    while (comp[n] != comp[comp[n]]) {
      auto foo = nw::graph::acquire(comp[n]);
      auto bar = nw::graph::acquire(comp[foo]);
      nw::graph::release(comp[n], bar);
    }
  });
}

template <typename Vector>
static vertex_id_type sample_frequent_element(const Vector& comp, size_t num_samples = 1024) {
  std::unordered_map<vertex_id_type, int>       counts(32);
  std::mt19937                                  gen;
  std::uniform_int_distribution<vertex_id_type> distribution(0, comp.size() - 1);

  for (size_t i = 0; i < num_samples; ++i) {
    vertex_id_type n = distribution(gen);
    counts[comp[n]]++;
  }

  auto&& [num, count] = *std::max_element(counts.begin(), counts.end(), [](auto&& a, auto&& b) { return std::get<1>(a) < std::get<1>(b); });
  float frac_of_graph = static_cast<float>(count) / num_samples;
  std::cout << "Skipping largest intermediate component (ID: " << num << ", approx. " << int(frac_of_graph * 100) << "% of the graph)\n";
  return num;
}

template <typename Execution, typename Graph1, typename Graph2>
static auto afforest(Execution&& exec, Graph1&& graph, Graph2&& t_graph, size_t neighbor_rounds = 2) {
  std::vector<std::atomic<vertex_id_type>> comp(graph.size() + 1);
  std::for_each(exec, cins::counting_iterator(0ul), cins::counting_iterator(comp.size()), [&](vertex_id_type n) { comp[n] = n; });
  auto g = graph.begin();
  for (size_t r = 0; r < neighbor_rounds; ++r) {
    std::for_each(exec, cins::counting_iterator(0ul), cins::counting_iterator(comp.size()), [&](vertex_id_type u) {
      if (r < (g[u]).size()) {
        link(u, std::get<0>(g[u].begin()[r]), comp);
      }
    });
    compress(exec, graph, comp);
  }

  vertex_id_type c = sample_frequent_element(comp);

  std::for_each(exec, cins::counting_iterator(0ul), cins::counting_iterator(comp.size()), [&](vertex_id_type u) {
    if (comp[u] == c) return;

    if (neighbor_rounds < g[u].size()) {
      for (auto v = g[u].begin() + neighbor_rounds; v != g[u].end(); ++v) {
        link(u, std::get<0>(*v), comp);
      }
    }

    if (t_graph.size() != 0) {
      for (auto&& [v] : (t_graph.begin())[u]) {
        link(u, v, comp);
      }
    }
  });

  compress(exec, g, comp);

  return comp;
}

template <typename Graph, typename Vector>
static void print_top_n(const Graph& g, Vector&& comp, size_t n = 5) {
  std::unordered_map<vertex_id_type, vertex_id_type> count;
  for (auto&& i : comp) {
    count[i] += 1;
  }
  auto                                                   k = std::min(n, count.size());
  std::vector<std::pair<vertex_id_type, vertex_id_type>> count_vector;
  count_vector.reserve(count.size());
  for (auto kvp : count) {
    count_vector.push_back(kvp);
  }
  std::sort(count_vector.begin(), count_vector.end(), [&](auto&& p, auto&& q) { return (std::get<1>(p) > std::get<1>(q)); });
  count_vector.resize(k);
  std::cout << k << " biggest clusters\n";
  for (auto [i, j] : count_vector) {
    std::cout << i << ": " << j << "\n";
  }
  std::cout << "There are " << count.size() << " components\n";
}

// Verifies CC result by performing a BFS from a vertex in each component
// - Asserts search does not reach a vertex with a different component label
// - If the graph is directed, it performs the search as if it was undirected
// - Asserts every vertex is visited (degree-0 vertex should have own label)
template <class Graph, class Transpose, class Vector>
static bool CCVerifier(const Graph& graph, Transpose&& xpose, Vector&& comp) {
  using NodeID = typename nw::graph::vertex_id<std::decay_t<Graph>>::type;
  std::unordered_map<NodeID, NodeID> label_to_source;
  for (auto&& [n] : plain_range(graph)) {
    label_to_source[comp[n]] = n;
  }
  std::vector<bool>   visited(graph.size() + 1);
  std::vector<NodeID> frontier;
  frontier.reserve(graph.size() + 1);
  auto g = graph.begin();
  auto x = xpose.begin();
  for (auto&& [curr_label, source] : label_to_source) {
    frontier.clear();
    frontier.push_back(source);
    visited[source] = true;
    for (auto it = frontier.begin(); it != frontier.end(); it++) {
      NodeID u = *it;
      for (auto&& [v] : g[u]) {
        if (comp[v] != curr_label) {
          return false;
        }
        if (!visited[v]) {
          visited[v] = true;
          frontier.push_back(v);
        }
      }
      if (u < xpose.size()) {
        for (auto&& [v] : x[u]) {
          if (comp[v] != curr_label) {
            return false;
          }
          if (!visited[v]) {
            visited[v] = true;
            frontier.push_back(v);
          }
        }
      }
    }
  }
  NodeID i = 0;
  for (auto&& visited : visited) {
    if (!visited) {
      //return false;
      ++i;
    }
  }
  if (0 < i) {
    std::cout << "unvisited " << i << " " << graph.size() + 1 << " ";
    return false;
  }
  return true;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> strings(argv + 1, argv + argc);
  auto                     args = docopt::docopt(USAGE, strings, true);

  bool verify  = args["--verify"].asBool();
  bool verbose = args["--verbose"].asBool();
  bool debug   = args["--debug"].asBool();
  long trials  = args["-n"].asLong() ?: 1;

  std::vector ids     = parse_ids(args["--version"].asStringList());
  std::vector threads = parse_n_threads(args["THREADS"].asStringList());

  std::vector<std::tuple<std::string, bool>> files;

  // normal files
  for (auto&& file : args["-f"].asStringList()) {
    files.emplace_back(file, false);
  }

  // symmetric files
  for (auto&& file : args["-s"].asStringList()) {
    files.emplace_back(file, true);
  }

  Times<bool> times;

  // Appease clang.
  //
  // These are captured in lambdas later, and if I use structured bindings
  // they have to be listed as explicit captures (this is according to the 17
  // standard). That's a little bit noisy where it happens, so I just give
  // them real symbols here rather than the local bindings.
  for (auto&& f : files) {
    auto reader = [&](std::string file, bool symmetric, bool verbose) {
      if (symmetric) {
        auto aos_a  = load_graph<nw::graph::directedness::undirected>(file);
        auto degree = degrees(aos_a);

        // Run relabeling. This operates directly on the incoming edglist.
        if (args["--relabel"].asBool()) {
          relabel_by_degree<0>(aos_a, args["--direction"].asString(), degree);
        }
        // Clean up the edgelist to deal with the normal issues related to
        // undirectedness.
        if (args["--clean"].asBool()) {
          swap_to_triangular<0>(aos_a, args["--succession"].asString());
          lexical_sort_by<0>(aos_a);
          uniq(aos_a);
          remove_self_loops(aos_a);
        }

        adjacency<0> graph(aos_a);
        adjacency<1> t_graph(0);
        if (verbose) graph.stream_stats();
        return std::tuple(graph, t_graph);
      }    //if
      else {
        auto aos_a  = load_graph<nw::graph::directedness::directed>(file);
        auto degree = degrees(aos_a);

        // Run and time relabeling. This operates directly on the incoming edglist.
        if (args["--relabel"].asBool()) {
          relabel_by_degree<0>(aos_a, args["--direction"].asString(), degree);
        }

        // Clean up the edgelist to deal with the normal issues related to
        // undirectedness.
        if (args["--clean"].asBool()) {
          swap_to_triangular<0>(aos_a, args["--succession"].asString());
          lexical_sort_by<0>(aos_a);
          uniq(aos_a);
          remove_self_loops(aos_a);
        }

        adjacency<0> graph(aos_a);
        adjacency<1> t_graph(aos_a);
        if (verbose) {
          graph.stream_stats();
          t_graph.stream_stats();
        }
        return std::tuple(graph, t_graph);
      }    //else
    };

    auto&& file      = std::get<0>(f);
    auto&& symmetric = std::get<1>(f);
    auto&& graphs    = reader(file, symmetric, verbose);
    auto&& graph     = std::get<0>(graphs);
    auto&& t_graph   = std::get<1>(graphs);

    if (verbose) {
      graph.stream_stats();
      if (t_graph.size() != 0) {
        t_graph.stream_stats();
      }
    }

    if (debug) {
      graph.stream_indices();
      if (t_graph.size() != 0) {
        t_graph.stream_indices();
      }
    }

    for (auto&& thread : threads) {
      auto _ = set_n_threads(thread);
      for (auto&& id : ids) {
        if (verbose) {
          std::cout << "version " << id << std::endl;
        }

        auto verifier = [&](auto&& comp) {
          if (verbose) {
            print_top_n(graph, comp);
          }
          if (verify && !CCVerifier(graph, t_graph, comp)) {
            std::cerr << " v" << id << " failed verification for " << file << " using " << thread << " threads\n";
          }
        };

        auto record = [&](auto&& op) { times.record(file, id, thread, std::forward<decltype(op)>(op), verifier, symmetric); };
        using Graph = adjacency<0>;

        for (int j = 0, e = trials; j < e; ++j) {
          switch (id) {
            case 0:
              record([&] { return afforest(std::execution::seq, graph, t_graph); });
              break;
            case 1:
              record([&] { return ccv1<Graph, vertex_id_type>(graph); });    //push
              break;
            case 2:
              record([&] { return compute_connected_components_v2<Graph, vertex_id_type>(graph); });    //pull
              break;
            case 5:
              record([&] { return ccv5<Graph, vertex_id_type>(graph); });    //pull + afforest
              break;
            case 6:
              record([&] { return sv_v6<Graph, vertex_id_type>(graph); });    //sv
              break;
            case 7:
              record([&] { return afforest(std::execution::par_unseq, graph, t_graph); });
              break;
            case 8:
              record([&] { return sv_v8<Graph, vertex_id_type>(graph); });    //sv
              break;
            case 9:
              record([&] { return sv_v9<Graph, vertex_id_type>(graph); });    //sv
              break;
            default:
              std::cout << "Unknown version v" << id << "\n";
          }
        }
      }
    }
  }

  times.print(std::cout);

  if (args["--log"]) {
    auto file   = args["--log"].asString();
    bool header = args["--log-header"].asBool();
    log("cc", file, times, header, "Time(s)", "Symmetric");
  }

  return 0;
}
