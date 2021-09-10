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

#include "algorithms/connected_components.hpp"
#include "Log.hpp"
#include "common.hpp"
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






template<typename Graph, typename Vector>
static void print_top_n(Graph&& g, Vector&& comp, size_t n = 5) {
  std::unordered_map<vertex_id_t, vertex_id_t> count;
  for (auto&& i : comp) {
    count[i] += 1;
  }
  auto k = std::min(n, count.size());
  std::vector<std::pair<vertex_id_t, vertex_id_t>> count_vector;
  count_vector.reserve(count.size());
  for (auto kvp : count) {
    count_vector.push_back(kvp);
  }
  std::sort(count_vector.begin(), count_vector.end(), [&](auto&& p, auto&& q) { return (std::get<1>(p) > std::get<1>(q)); });
  count_vector.resize(k);
  std::cout << k << " biggest clusters\n";
  for (auto [i, j]: count_vector) {
    std::cout << i << ": " << j << "\n";
  }
  std::cout << "There are " << count.size() << " components\n";
}

static auto read_graphs(std::string path, bool symmetric, bool verbose) {
  if (symmetric) {
    auto aos_a = load_graph<undirected>(path);
    if (verbose) {
      aos_a.stream_stats();
    }
    return std::tuple(build_adjacency<0>(aos_a), adjacency<1>(0));
  }
  else {
    auto aos_a = load_graph<directed>(path);
    if (verbose) {
      aos_a.stream_stats();
    }
    return std::tuple(build_adjacency<0>(aos_a), build_adjacency<1>(aos_a));
  }
}


// Verifies CC result by performing a BFS from a vertex in each component
// - Asserts search does not reach a vertex with a different component label
// - If the graph is directed, it performs the search as if it was undirected
// - Asserts every vertex is visited (degree-0 vertex should have own label)
template <class Graph, class Transpose, class Vector>
static bool CCVerifier(Graph&& graph, Transpose&& xpose, Vector&& comp) {
  using NodeID = typename nw::graph::vertex_id<std::decay_t<Graph>>::type;
  std::unordered_map<NodeID, NodeID> label_to_source;
  for (auto&& [n] : plain_range(graph)) {
    label_to_source[comp[n]] = n;
  }
  std::vector<bool> visited(graph.max() + 1);
  std::vector<NodeID> frontier;
  frontier.reserve(graph.max() + 1);
  auto g = graph.begin();
  auto x = xpose.begin();
  for (auto&& [curr_label, source] : label_to_source)
  {
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
  if (0 < i){
    std::cout << "unvisited " << i << " " << graph.max() + 1 << " ";
    return false;
  }
  return true;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> strings(argv + 1, argv + argc);
  auto args = docopt::docopt(USAGE, strings, true);

  bool    verify = args["--verify"].asBool();
  bool   verbose = args["--verbose"].asBool();
  bool     debug = args["--debug"].asBool();
  long    trials = args["-n"].asLong() ?: 1;

  std::vector        ids = parse_ids(args["--version"].asStringList());
  std::vector    threads = parse_n_threads(args["THREADS"].asStringList());

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
  for (auto&& f : files)
  {
    auto reader = [&](std::string file, bool symmetric, bool verbose) {
      if (symmetric) {
        auto aos_a = load_graph<undirected>(file);
        auto degrees = aos_a.degrees();

        // Run relabeling. This operates directly on the incoming edglist.
        if (args["--relabel"].asBool()) {
          aos_a.relabel_by_degree<0>(args["--direction"].asString(), degrees);
        }
      // Clean up the edgelist to deal with the normal issues related to
      // undirectedness.
        if (args["--clean"].asBool()) {
          aos_a.swap_to_triangular<0>(args["--succession"].asString());
          aos_a.lexical_sort_by<0>();
          aos_a.uniq();
          aos_a.remove_self_loops();
        }

        adjacency<0> graph(aos_a);
        adjacency<1> t_graph(0);
        if (verbose) graph.stream_stats();
        return std::tuple(graph, t_graph);
      } //if
      else {
        auto aos_a = load_graph<directed>(file);
        auto degrees = aos_a.degrees();

        // Run and time relabeling. This operates directly on the incoming edglist.
        if (args["--relabel"].asBool()) {
          aos_a.relabel_by_degree<0>(args["--direction"].asString(), degrees);
        }

        // Clean up the edgelist to deal with the normal issues related to
        // undirectedness.
        if (args["--clean"].asBool()) {
          aos_a.swap_to_triangular<0>(args["--succession"].asString());
          aos_a.lexical_sort_by<0>();
          aos_a.uniq();
          aos_a.remove_self_loops();
        }
      
        adjacency<0> graph(aos_a);
        adjacency<1> t_graph(aos_a);
        if (verbose) {
          graph.stream_stats();
          t_graph.stream_stats();
        }
        return std::tuple(graph, t_graph);
      } //else
    };

    auto&&      file = std::get<0>(f);
    auto&& symmetric = std::get<1>(f);
    auto&&    graphs = reader(file, symmetric, verbose);
    auto&&     graph = std::get<0>(graphs);
    auto&&   t_graph = std::get<1>(graphs);

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
            std::cerr << " v" << id << " failed verification for " << file
                      << " using " << thread << " threads\n";
          }
        };

        auto record = [&](auto&& op) {
          times.record(file, id, thread, std::forward<decltype(op)>(op), verifier, symmetric);
        };

        for (int j = 0, e = trials; j < e; ++j) {
          switch (id) {
           case 0: record([&] { return afforest(std::execution::seq, graph, t_graph); });
            break;
           case 1: record([&] { return ccv1(graph); }); //push
            break;
           case 2: record([&] { return compute_connected_components_v2(graph); }); //pull
            break;
           case 5: record([&] { return ccv5(graph); }); //pull + afforest
            break;
           case 6: record([&] { return sv_v6(graph); }); //sv
            break;
           case 7: record([&] { return afforest(std::execution::par_unseq, graph, t_graph); });
            break;
           case 8: record([&] { return sv_v8(graph); }); //sv
            break;
           case 9: record([&] { return sv_v9(graph); }); //sv
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
    auto   file = args["--log"].asString();
    bool header = args["--log-header"].asBool();
    log("cc", file, times, header, "Time(s)", "Symmetric");
  }
  

  return 0;
}
