//
// This file is part of BGL17 (aka NWGraph aka GraphPack aka the Graph Standard Library)
// (c) Pacific Northwest National Laboratory 2018
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#include <iostream>
#include <queue>

#include "adaptors/bfs_edge_range.hpp"
#include "adaptors/bfs_range.hpp"
#include "containers/aos.hpp"

#include "common/abstract_test.hpp"

using namespace nw::graph;
using namespace nw::util;

//****************************************************************************
template <typename EdgeListT>
bool validate(EdgeListT& aos, size_t seed, std::vector<size_t> const& distance, std::vector<size_t> const& predecessor) {
  bool pass(true);

  /// @note Should we parse the orginal mmio file instead?
  for (auto&& [u, v] : aos) {
    int    localdiff = distance[u] - distance[v];
    size_t diff      = abs(localdiff);
#ifdef PRINT_OUT
    std::cout << "Checking edge: " << u << " (" << distance[u] << ", " << predecessor[u] << ") -> " << v << " (" << distance[v]
              << ", " << predecessor[v] << "), diff = " << diff << std::endl;
#endif
    if (diff > 1) {
#ifdef PRINT_OUT
      std::cerr << "ERROR: too far: dist[" << u << "] = " << distance[u] << " too far from dist[" << v << "] = " << distance[v]
                << std::endl;
#endif
      pass = false;
      break;
    }
  }

  //std::cout << "vertex\tdepth\tparent\tpass-fail" << std::endl;
  for (size_t vid = 0; vid < distance.size(); ++vid) {
    bool passed = ((vid == seed) && (distance[vid] == 0)) || (distance[vid] == (1 + distance[predecessor[vid]]));
    pass &= passed;
    //    std::cout << vid+1
    //            << ":\t" << distance[vid]
    //              << "\t" << predecessor[vid]+1
    //              << "\t" << (passed ? "PASS" : "FAIL") << std::endl;
  }

  return pass;
}

TEST_CASE("BFS traversal", "[bfs]") {

  test_util            util;
  auto                 aos_a = util.generate_directed_aos();
  directed_csr_graph_t A(aos_a);

  size_t              N = A.size();
  std::vector<size_t> distance(N);
  std::vector<size_t> predecessor(N);
  /*
  SECTION("default seed") {
    vertex_id_type seed = 0;
    distance[seed]      = 0;
    bfs_edge_range ranges(A, seed);
    auto           ite = ranges.begin();
    for (; ite != ranges.end(); ++ite) {
      auto v         = std::get<0>(*ite);
      auto u         = std::get<1>(*ite);
      distance[u]    = distance[v] + 1;
      predecessor[u] = v;
    }

    REQUIRE(validate(aos_a, seed, distance, predecessor));
  }
*/
  SECTION("another seed") {
    vertex_id_t<directed_csr_graph_t> seed = 1;
    distance[seed]                         = 0;
    bfs_edge_range ranges(A, seed);
    auto           ite = ranges.begin();
    for (; ite != ranges.end(); ++ite) {
      auto v      = std::get<0>(*ite);
      auto u      = std::get<1>(*ite);
      distance[u] = distance[v] + 1;
      std::cout << u << "-" << v << ":\t" << distance[u] << ":\t" << distance[v] << std::endl;
      predecessor[u] = v;
    }

    REQUIRE(validate(aos_a, seed, distance, predecessor));
  }
  /*
  SECTION("Top-down BFS default seed using bfs_range") {
    vertex_id_type seed = 0;
    distance[seed]      = 0;
    bfs_range ranges(A, seed);
    auto      ite = ranges.begin();
    for (; ite != ranges.end(); ++ite) {
      auto v         = std::get<0>(*ite);
      auto u         = std::get<1>(*ite);
      distance[u]    = distance[v] + 1;
      predecessor[u] = v;
    }

    REQUIRE(validate(aos_a, seed, distance, predecessor));
  }
    SECTION("Top-down BFS another seed using bfs_range") {
    vertex_id_type seed = 7;
    distance[seed]      = 0;
    bfs_range ranges(A, seed);
    auto      ite = ranges.begin();
    for (; ite != ranges.end(); ++ite) {
      auto v         = std::get<0>(*ite);
      auto u         = std::get<1>(*ite);
      distance[u]    = distance[v] + 1;
      predecessor[u] = v;
    }

    REQUIRE(validate(aos_a, seed, distance, predecessor));
  }

    SECTION("Bottom-up BFS default seed using bottomup_bfs_range") {
    vertex_id_type seed = 0;
    distance[seed]      = 0;
    bottomup_bfs_range ranges(A, seed);
    
    auto      ite = ranges.begin();
    for (; ite != ranges.end(); ++ite) {
      auto v         = std::get<0>(*ite);
      auto u         = std::get<1>(*ite);
      distance[u]    = distance[v] + 1;
                std::cout << u << "-" << v
            << ":\t" << distance[u]
            << ":\t" << distance[v] << std::endl;
      predecessor[u] = v;
          std::cout << u
            << ":\t" << distance[u]
              << "\t" << predecessor[u] << std::endl;
    }
      for (size_t vid = 0; vid < distance.size(); ++vid) {
    std::cout << vid
            << ":\t" << distance[vid]
              << "\t" << predecessor[vid] << std::endl;
  }
    REQUIRE(validate(aos_a, seed, distance, predecessor));
  }
  
    SECTION("Bottom-up BFS another seed using bottomup_bfs_range") {
    vertex_id_type seed = 1;
    distance[seed]      = 0;
    bottomup_bfs_range ranges(A, seed);
    auto      ite = ranges.begin();
    for (; ite != ranges.end(); ++ite) {
      auto v         = std::get<0>(*ite);
      auto u         = std::get<1>(*ite);
      distance[u]    = distance[v] + 1;
                  std::cout << u << "-" << v
            << ":\t" << distance[u]
            << ":\t" << distance[v] << std::endl;
      predecessor[u] = v;
    }


    REQUIRE(validate(aos_a, seed, distance, predecessor));
  }
 */
}
