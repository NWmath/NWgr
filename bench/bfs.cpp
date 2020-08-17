// This file is part of the Graph Standard Library (aka BGL17 aka NWGraph)
//
// (c) Pacific Northwest National Laboratory 2020
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//
static constexpr const char USAGE[] =
    R"(bfs.exe: BGL17 breadth first search benchmark driver.
  Usage:
      bfs.exe (-h | --help)
      bfs.exe -f FILE [-r NODE | -s FILE] [-i NUM] [-a NUM] [-b NUM] [-B NUM] [-n NUM] [--seed NUM] [--version ID...] [--log FILE] [--log-header] [-dvV] [THREADS]...

  Options:
      -h, --help              show this screen
      -f FILE                 input file path
      -i NUM                  number of iteration [default: 1]
      -a NUM                  alpha parameter [default: 15]
      -b NUM                  beta parameter [default: 18]
      -B NUM                  number of bins [default: 32]
      -n NUM                  number of trials [default: 1]
      -r NODE                 start from node r (default is random)
      -s, --sources FILE      sources file
      --seed NUM              random seed [default: 27491095]
      --version ID            algorithm version to run [default: 0]
      --log FILE              log times to a file
      --log-header            add a header to the log file
      -d, --debug             run in debug mode
      -v, --verify            verify results
      -V, --verbose           run in verbose mode
)";

#include "algorithms/bfs.hpp"
#include "Log.hpp"
#include "common.hpp"
#include <docopt.h>

using namespace bgl17::bench;

template<typename Graph, typename GraphT>
bool BFSVerifier(Graph& g, GraphT& g_t, vertex_id_t source, std::vector<vertex_id_t>& parent) {
  std::vector<vertex_id_t> depth(g.max() + 1, std::numeric_limits<vertex_id_t>::max());
  depth[source] = 0;
  std::vector<vertex_id_t> to_visit;
  to_visit.reserve(g.max() + 1);
  to_visit.push_back(source);
  auto out_neigh = g.begin();
  auto in_neigh  = g_t.begin();
  for (auto it = to_visit.begin(); it != to_visit.end(); it++) {
    vertex_id_t u = *it;
    for (auto edge : out_neigh[u]) {
      vertex_id_t v = std::get<0>(edge);
      if (depth[v] == std::numeric_limits<vertex_id_t>::max()) {
        depth[v] = depth[u] + 1;
        to_visit.push_back(v);
      }
    }
  }
  for (vertex_id_t u = 0; u < g.max() + 1; ++u) {
    if ((depth[u] != std::numeric_limits<vertex_id_t>::max()) && (parent[u] != std::numeric_limits<vertex_id_t>::max())) {
      if (u == source) {
        if (!((parent[u] == u) && (depth[u] == 0))) {
          std::cout << "Source wrong " << u << " " << parent[u] << " " << depth[u] << std::endl;
          return false;
        }
        continue;
      }
      bool parent_found = false;
      for (auto edge : in_neigh[u]) {
        vertex_id_t v = std::get<0>(edge);
        if (v == parent[u]) {
          //if(it != out_neigh[v].end()) {
          if (depth[v] != depth[u] - 1) {
            std::cout << "Wrong depths for " << u << " & " << v << std::endl;
            return false;
          }
          parent_found = true;
          break;
        }
      }
      if (!parent_found) {
        std::cout << "Couldn't find edge from " << parent[u] << " to " << u << std::endl;
        return false;
      }
    } else if (depth[u] != parent[u]) {
      std::cout << "Reachability mismatch " << u << " " << depth[u] << " " << parent[u] << std::endl;
      return false;
    }
  }
  return true;
}

int main(int argc, char* argv[]) {
  std::vector strings = std::vector<std::string>(argv + 1, argv + argc);
  std::map    args    = docopt::docopt(USAGE, strings, true);

  // Read the options
  bool        verify     = args["--verify"].asBool();
  bool        verbose    = args["--verbose"].asBool();
  bool        debug      = args["--debug"].asBool();
  long        trials     = args["-n"].asLong() ?: 1;
  long        iterations = args["-i"].asLong() ?: 1;
  long        alpha      = args["-a"].asLong() ?: 15;
  long        beta       = args["-b"].asLong() ?: 18;
  long        num_bins   = args["-B"].asLong() ?: 32;
  std::string file       = args["-f"].asString();

  std::vector ids     = parse_ids(args["--version"].asStringList());
  std::vector threads = parse_n_threads(args["THREADS"].asStringList());

  auto aos_a = load_graph<directed>(file);

  if (verbose) {
    aos_a.stream_stats();
  }

  auto graph = build_adjacency<1>(aos_a);
  auto    gx = build_adjacency<0>(aos_a);

  if (verbose) {
    graph.stream_stats();
  }

  if (debug) {
    graph.stream_indices();
  }

  std::vector<vertex_id_t> sources;
  if (args["--sources"]) {
    sources = load_sources_from_file(graph, args["--sources"].asString());
    trials  = sources.size();
  } else if (args["-r"]) {
    sources.resize(trials);
    std::fill(sources.begin(), sources.end(), args["-r"].asLong());
  } else {
    sources = build_random_sources(graph, trials, args["--seed"].asLong());
  }

  Times<vertex_id_t> times;

  std::map<long, std::vector<size_t>> levels;

  for (auto&& thread : threads) {
    auto _ = set_n_threads(thread);
    for (auto&& id : ids) {
      for (auto&& source : sources) {
        if (verbose) {
          std::cout << "source: " << source << "\n";
        }

        auto&& [time, parents] = time_op([&] {
            switch (id) {
             case  0: return bfs_v0(graph, source);
             case  6: return bfs_v6(graph, source);
             case  7: return bfs_v7(graph, source);
             case  8: return bfs_v8(graph, source);
             case  9: return bfs_v9(graph, source);
             case 10: return bfs_top_down(graph, source);
	     case 11: return bfs_v11(graph, gx, source, num_bins, alpha, beta);
             case 12: return bfs_top_down_bitmap(graph, source);
             case 13: return bfs_bottom_up(graph, gx, source);
             default:
              std::cerr << "Unknown version " << id << "\n";
              return std::vector<vertex_id_t>();
            }
          });

        if (verify) {
          BFSVerifier(graph, gx, source, parents);
        }

        times.append(file, id, thread, time, source);
      }
    }
  }

  times.print(std::cout);

  if (args["--log"]) {
    auto file   = args["--log"].asString();
    bool header = args["--log-header"].asBool();
    log("bfs", file, times, header, "Time(s)", "Source");
  }

  return 0;
}
