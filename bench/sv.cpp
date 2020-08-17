//
// This file is part of the Graph Standard Library (aka BGL17 aka NWGraph)
// (c) 2020 Pacific Northwest National Laboratory
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#include <iomanip>
#include <iostream>
#include <queue>
#include <random>
#include <vector>

#include "aolos.hpp"
#include "compressed.hpp"
#include "edge_list.hpp"

#include "mmio.hpp"

#include "algorithms/connected_component.hpp"
#include "util.hpp"
#include "util/timer.hpp"
#include "util/types.hpp"


template<typename HyperGraph>
auto hsv(HyperGraph& g) {

  std::vector<vertex_id_t> E(std::get<0>(g.max2()));
  std::vector<vertex_id_t> N(std::get<1>(g.max2()));

  std::for_each(std::execution::par_unseq,
      counting_iterator<vertex_id_t>(0), counting_iterator<vertex_id_t>(g.size()), [&](auto n) { E[n] = n; });
  std::for_each(std::execution::par_unseq,
      counting_iterator<vertex_id_t>(0), counting_iterator<vertex_id_t>(g.size()), [&](auto n) { N[n] = n; });

  bool change = true;

  auto G = g.begin();

  for (size_t num_iter = 0; num_iter < g.size(); ++num_iter) {
    if (false == change) {
      break;
    }
    change = false;

    std::for_each(
        std::execution::par_unseq,
        counting_iterator<vertex_id_t>(0), counting_iterator<vertex_id_t>(g.size()), [&](auto u) {
          for (auto&& [v] : G[u]) {
            vertex_id_t comp_u = E[u];
            vertex_id_t comp_v = N[v];

            if (comp_u == comp_v) continue;

            vertex_id_t high_comp = comp_u > comp_v ? comp_u : comp_v;
            vertex_id_t low_comp  = comp_u + (comp_v - high_comp);

            if (high_comp == E[high_comp]) {
              change          = true;
              E[high_comp] = low_comp;
            }

            if (high_comp == N[high_comp]) {
              change          = true;
              N[high_comp] = low_comp;
            }

          }
        });

    for (vertex_id_t n = 0; n < g.size(); n++) {
      while (E[n] != E[E[n]]) {
        E[n] = E[E[n]];
      }
    }
    for (vertex_id_t n = 0; n < g.size(); n++) {
      while (N[n] != N[N[n]]) {
        N[n] = N[N[n]];
      }
    }
  }

  return std::tuple(E, N);
}





void usage(const std::string& msg = "") { std::cout << std::string("Usage: ") + msg + " " << std::endl; }

int main(int argc, char* argv[]) {

  std::string edgelistFile             = "";
  std::string versions                 = "0";
  std::string read_processed_edgelist  = "";
  std::string write_processed_edgelist = "";

  bool         verbose      = false;
  bool         debug        = false;
  size_t       nthread      = 1; (void)nthread; // silence warnings
  size_t       ntrial       = 1;
  size_t       max_iters    = 20; (void)max_iters; // silence warnings
  const size_t max_versions = 16;

  for (int argIndex = 1; argIndex < argc; ++argIndex) {
    std::string arg(argv[argIndex]);

    if (arg == "--edgelistfile" || arg == "-f" || arg == "-i") {
      if (++argIndex == argc) {
        usage(argv[0]);
      }
      edgelistFile = std::string(argv[argIndex]);
    } else if (arg == "--read_processed_edgelist") {
      if (++argIndex == argc) {
        usage(argv[0]);
      }
      read_processed_edgelist = std::string(argv[argIndex]);
    } else if (arg == "--write_processed_edgelist") {
      if (++argIndex == argc) {
        usage(argv[0]);
      }
      write_processed_edgelist = std::string(argv[argIndex]);
    } else if (arg == "--ntrial" || arg == "--ntrials") {
      if (++argIndex == argc) {
        usage(argv[0]);
      }
      ntrial = std::stoi(argv[argIndex]);
    } else if (arg == "--nthread" || arg == "--nthreads") {
      if (++argIndex == argc) {
        usage(argv[0]);
      }
      nthread = std::stoi(argv[argIndex]);
    } else if (arg == "-m" || arg == "--maxiter" || arg == "--maxiters" || arg == "--max_iter" || arg == "--max_iters") {
      if (++argIndex == argc) {
        usage(argv[0]);
      }
      max_iters = std::stoi(argv[argIndex]);
    } else if (arg == "--version" || arg == "--versions") {
      if (++argIndex == argc) {
        usage(argv[0]);
      }
      versions = std::string(argv[argIndex]);
    } else if (arg == "-d") {
      debug = true;
    } else if (arg == "-v") {
      verbose = true;
    } else {
      usage(argv[0]);
      return -1;
    }
  }

  auto aos_a = [&]() {
    if (read_processed_edgelist != "") {
      life_timer          _("deserialize");
      edge_list<directed> aos_a(0);
      aos_a.deserialize(read_processed_edgelist);
      return aos_a;
    } else /* if (edgelistFile != "") */ {
      life_timer _("read mm");
      return read_mm<directed>(edgelistFile);
    }
  }();

  if (verbose) {
    aos_a.stream_stats();
  }

  if (write_processed_edgelist != "") {
    aos_a.serialize(write_processed_edgelist);
  }

  auto graph = [&]() {
    life_timer _("build");
    return adjacency<1>(aos_a);
  }();

  if (verbose) {
    graph.stream_stats();
  }
  if (debug) {
    graph.stream_indices();
  }

  std::vector<float> components(graph.size());

  std::chrono::duration<double> timings[max_versions][ntrial];
  for (const char* c = versions.c_str(); *c != 0; ++c) {

    size_t num = *c - '0';    // old C hack
    if (verbose) {
      std::cout << "version " << num << std::endl;
    }

    for (size_t trial = 0; trial < ntrial; ++trial) {
      auto start = std::chrono::system_clock::now();
      switch (num) {

        case 0:
          // page_rank_range_for(graph, page_rank, 0.85f, tolerance, max_iters);
          break;

        case 10:
          // sv_vc(graph);
          break;

        case 1:
          // sv_v1(graph);
          break;

        case 4:
          // sv_v4(graph);
          break;

        case 6:
          hsv(graph);
          break;

        case 8:
          // sv_v8(graph);
          break;

        case 9:
          // sv_v9(graph);
          break;

        default:
          std::cout << std::string("Unknown version num ") + std::to_string(num) << std::endl;
          break;
      }
      timings[num][trial] = std::chrono::system_clock::now() - start;
    }
  }

  for (const char* c = versions.c_str(); *c != 0; ++c) {
    size_t num      = *c - '0';
    double avg_time = 0;
    for (size_t i = 0; i < ntrial; ++i) {
      avg_time += timings[num][i].count();
    }
    avg_time /= ntrial;
    std::cout << "v" + std::to_string(num) + ": ";
    std::cout << (*(std::min_element(timings[num], timings[num] + ntrial))).count() << " (min), ";
    std::cout << avg_time << "(avg), ";
    std::cout << (*(std::max_element(timings[num], timings[num] + ntrial))).count() << " (max)\n";
  }

  return 0;
}
