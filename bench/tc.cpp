//****************************************************************************
// Copyright 2014, Software Engineering Institute, Carnegie Mellon University
//
// File: [filename]
//
// Description:
//****************************************************************************
//
// This file is part of BGL17 (aka NWGraph aka GraphPack aka the Graph Standard Library)
// (c) Pacific Northwest National Laboratory 2020-2021
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Kevin Deweese, Andrew Lumsdaine
//
static constexpr const char USAGE[] =
    R"(tc.exe: BGL17 page rank benchmark driver.
  Usage:
      tc.exe (-h | --help)
      tc.exe -f FILE... [--version ID...] [-n NUM] [--lower | --upper] [--relabel] [--heuristic] [--log FILE] [--log-header] [-dvV] [THREADS...]

  Options:
      -h, --help            show this screen
      --version ID          algorithm version to run [default: 4]
      -f FILE               input file path
      -n NUM                number of trials [default: 1]
      --lower               lower triangular order [default: false]
      --upper               upper triangular order [default: true]
      --relabel             relabel the graph
      --heuristic           use heuristic to decide whether to relabel or not
      --log FILE            log times to a file
      --log-header          add a header to the log file
      -d, --debug           run in debug mode
      -v, --verify          verify results
      -V, --verbose         run in verbose mode
)";

#include "containers/adjacency.hpp"
#include "containers/edge_list.hpp"

#include "Log.hpp"
#include "algorithms/triangle_count.hpp"
#include "common.hpp"
#include <docopt.h>
#include <tuple>

using namespace nw::graph::bench;
using namespace nw::graph;
using namespace nw::util;

template <class Vector>
static void tc_relabel(edge_list<nw::graph::directedness::undirected>& A, Vector&& degrees, const std::string& direction) {
  life_timer _(__func__);
  relabel_by_degree<0>(A, direction, degrees);
}

template <std::size_t id = 0>
static void clean(edge_list<nw::graph::directedness::undirected>& A, const std::string& succession) {
  life_timer _(__func__);
  swap_to_triangular<id>(A, succession);
  lexical_sort_by<id>(A);
  uniq(A);
  remove_self_loops(A);
}

static auto compress(edge_list<nw::graph::directedness::undirected>& A) {
  life_timer   _(__func__);
  adjacency<0> B(num_vertices(A));
  push_back_fill(A, B);
  return B;
}

// heuristic to see if sufficently dense power-law graph
template <class EdgeList, class Vector>
static bool worth_relabeling(const EdgeList& el, const Vector& degree) {
  using vertex_id_type = typename EdgeList::vertex_id_type;

  int64_t average_degree = el.size() / (el.num_vertices()[0]);
  if (average_degree < 10) return false;

  int64_t              num_samples  = std::min<int64_t>(1000L, el.num_vertices()[0]);
  int64_t              sample_total = 0;
  std::vector<int64_t> samples(num_samples);

  std::mt19937                                  rng;
  std::uniform_int_distribution<vertex_id_type> udist(0, el.num_vertices()[0] - 1);

  for (int64_t trial = 0; trial < num_samples; trial++) {
    samples[trial] = degree[udist(rng)];
    sample_total += samples[trial];
  }
  std::sort(std::execution::par_unseq, samples.begin(), samples.end());
  double sample_average = static_cast<double>(sample_total) / num_samples;
  double sample_median  = samples[num_samples / 2];
  return sample_average / 1.3 > sample_median;
}

// Taken from GAP and adapted to NW Graph
template <class Graph>
static std::size_t TCVerifier(Graph& graph) {
  using vertex_id_type = typename Graph::vertex_id_type;

  life_timer                              _(__func__);
  std::size_t                             total = 0;
  std::vector<std::tuple<vertex_id_type>> intersection;
  intersection.reserve(graph.size());
  auto g = graph.begin();
  for (auto&& [u, v] : edge_range(graph)) {
    auto u_out = g[u];
    auto v_out = g[v];
    auto end   = std::set_intersection(u_out.begin(), u_out.end(), v_out.begin(), v_out.end(), intersection.begin());
    intersection.resize(end - intersection.begin());
    total += intersection.size();
  }
  return total;    // note that our processed Graph doesn't produce extra counts
                   // like the GAP verifier normally would
}

int main(int argc, char* argv[]) {
  std::vector<std::string> strings(argv + 1, argv + argc);
  auto                     args = docopt::docopt(USAGE, strings, true);

  // Read the easy options
  bool verify  = args["--verify"].asBool();
  bool verbose = args["--verbose"].asBool();
  bool debug   = args["--debug"].asBool();
  long trials  = args["-n"].asLong() ?: 1;

  // Read the more complex options
  std::string direction  = "ascending";
  std::string succession = "successor";
  if (args["--lower"].asBool()) {
    direction  = "descending";
    succession = "predecessor";
  }

  std::vector files   = args["-f"].asStringList();
  std::vector ids     = parse_ids(args["--version"].asStringList());
  std::vector threads = parse_n_threads(args["THREADS"].asStringList());

  Times<double, double, double, std::size_t, bool> times;

  for (auto&& file : files) {
    std::cout << "processing " << file << "\n";

    auto el_a   = load_graph<nw::graph::directedness::undirected>(file);
    auto degree = degrees(el_a);

    // Run and time relabeling. This operates directly on the incoming edglist.
    bool relabeled        = false;
    auto&& [relabel_time] = time_op([&] {
      if (args["--relabel"].asBool()) {
        if (args["--heuristic"].asBool() == false || worth_relabeling(el_a, degree)) {
          tc_relabel(el_a, degree, direction);
          relabeled = true;
        }
      }
    });

    // Force relabel time to 0 if we didn't relabel, this just suppresses
    // nanosecond noise from the time_op function when relabeling wasn't done.
    if (!relabeled) {
      relabel_time = 0.0;
    }

    // Clean up the edgelist to deal with the normal issues related to
    // undirectedness.
    auto&& [clean_time] = time_op([&] { clean<0>(el_a, succession); });

    // Create the CSR from the undirected edgelist.
    auto cel_a = compress(el_a);

    if (debug) {
      cel_a.stream_indices();
    }

    // If we're verifying then compute the number of triangles once for this
    // graph.
    std::size_t v_triangles = 0;
    if (verify) {
      v_triangles = TCVerifier(cel_a);
      std::cout << "verifier reports " << v_triangles << " triangles\n";
    }

    for (auto&& thread : threads) {
      auto _ = set_n_threads(thread);
      for (auto&& id : ids) {
        for (int j = 0; j < trials; ++j) {
          if (verbose) {
            std::cout << "running version:" << id << " threads:" << thread << "\n";
          }

          auto&& [time, triangles] = time_op([&]() -> std::size_t {
            switch (id) {
              case 0:
                return triangle_count_v0(cel_a);
              case 1:
                return triangle_count_v1(cel_a);
              case 2:
                return triangle_count_v2(cel_a);
              case 3:
                return triangle_count_v3(cel_a);
              case 4:
                return triangle_count_v4(cel_a.begin(), cel_a.end(), thread);
              case 5:
                return triangle_count_v5(cel_a.begin(), cel_a.end(), thread);
              case 6:
                return triangle_count_v6(cel_a.begin(), cel_a.end(), thread);
              case 7:
                return triangle_count_v7(cel_a);
              case 8:
                return triangle_count_v7(cel_a, std::execution::seq, std::execution::par_unseq);
              case 9:
                return triangle_count_v7(cel_a, std::execution::par_unseq, std::execution::par_unseq);
              case 10:
                return triangle_count_v10(cel_a);
              case 11:
                return triangle_count_v10(cel_a, std::execution::par_unseq, std::execution::par_unseq, std::execution::par_unseq);
              case 12:
                return triangle_count_v12(cel_a, thread);
              case 13:
                return triangle_count_v13(cel_a, thread);
              case 14:
                return triangle_count_v14(cel_a);
              case 15:
                return triangle_count_edgesplit(cel_a, thread);
              case 16:
                return triangle_count_edgesplit_upper(cel_a, thread);
#ifdef ONE_DIMENSIONAL_EDGE
              case 17:
                return triangle_count_edgerange(cel_a);
              case 18:
                return triangle_count_edgerange_cyclic(cel_a, thread);
#endif
              default:
                std::cerr << "Unknown version id " << id << "\n";
                return 0ul;
            }
          });

          // Add relabel time to total time, record total, solve, relabel, and
          // clean time, as well as the number of triangles and if we relabeled
          // this graph.
          times.append(file, id, thread, time + relabel_time, time, relabel_time, clean_time, triangles, relabeled);

          if (verify && triangles != v_triangles) {
            std::cerr << "Inconsistent results: v" << id << " failed verification for " << file << " using " << thread << " threads (reported "
                      << triangles << ")\n";
          }
        }
      }
    }
  }

  times.print(std::cout);

  if (args["--log"]) {
    auto file   = args["--log"].asString();
    bool header = args["--log-header"].asBool();
    log("tc", file, times, header, "Time(s)", "Solve(s)", "Relabel(s)", "Clean(s)", "Triangles", "Relabeled");
  }

  return 0;
}
