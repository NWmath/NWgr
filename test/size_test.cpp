//
// This file is part of BGL17 (aka NWGraph aka GraphPack aka the Graph Standard Library)
// (c) Pacific Northwest National Laboratory 2018
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Kevin Deweese
//

#include <algorithm>
#include <vector>

#include "nwgraph/containers/compressed.hpp"
#include "nwgraph/edge_list.hpp"
#include "nwgraph/volos.hpp"
#include "nwgraph/io/mmio.hpp"

#include "common/test_header.hpp"

using namespace nw::graph;

TEST_CASE("Size Test", "[size test]") {
  size_t                              N = 5;
  edge_list<directedness::undirected> E_list(N);
  E_list.close_for_push_back();


  REQUIRE(num_vertices(E_list) == 5);
  REQUIRE(E_list.size() == 0);
  E_list.open_for_push_back();
  REQUIRE(num_vertices(E_list) == 5);
  E_list.close_for_push_back();
  E_list.close_for_push_back();
  E_list.close_for_push_back();
  REQUIRE(num_vertices(E_list) == 5);
  REQUIRE(E_list.size() == 0);

  E_list.open_for_push_back();
  E_list.push_back(0, 1);
  E_list.push_back(1, 2);
  E_list.push_back(2, 3);

  REQUIRE(E_list.size() == 3);
  E_list.close_for_push_back();
  REQUIRE(E_list.size() == 3);
  REQUIRE(num_vertices(E_list) == 5);

  E_list.open_for_push_back();
  E_list.push_back(2, 6);
  E_list.close_for_push_back();
  REQUIRE(E_list.size() == 4);
  REQUIRE(num_vertices(E_list) == 7);

  adjacency<0> A(E_list);
  REQUIRE(A.size() == 7);
  REQUIRE(num_vertices(A) == 7);

  adj_list<0> A_list(E_list);
  REQUIRE(A_list.size() == 7);

  edge_list<directedness::directed> E_list2(0);
  E_list2.push_back(0, 1);
  E_list2.push_back(1, 2);
  E_list2.push_back(2, 3);
  E_list.close_for_push_back();

  adjacency<0> A2(E_list2);
  REQUIRE(A2.size() == 4);

  adj_list<0> A_list2(E_list2);
  REQUIRE(A_list2.size() == 4);
}
