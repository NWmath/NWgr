//
// This file is part of BGL17 (aka NWGraph aka GraphPack aka the Graph Standard Library)
// (c) Pacific Northwest National Laboratory 2018
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Kevin Deweese
//

#include <iostream>

#include <algorithms/triangle_count.hpp>
#include <compressed.hpp>
#include <edge_list.hpp>
#include <io/mmio.hpp>
#include <execution>
#include <unordered_set>
#include "common/test_header.hpp"
#include "rcm.hpp"

using namespace nw::graph;
using namespace nw::util;


TEST_CASE("Reverse Cuthill-Mckee Ordering", "[rcm]") {
  edge_list<directed> el(10);
  el.push_back(0, 1);
  el.push_back(0, 6);
  el.push_back(0, 8);
  el.push_back(1, 0);
  el.push_back(1, 4);
  el.push_back(1, 6);
  el.push_back(1, 9);
  el.push_back(2, 4);
  el.push_back(2, 6);
  el.push_back(3, 4);
  el.push_back(3, 5);
  el.push_back(3, 8);
  el.push_back(4, 1);
  el.push_back(4, 2);
  el.push_back(4, 3);
  el.push_back(4, 5);
  el.push_back(4, 9);
  el.push_back(5, 3);
  el.push_back(5, 4);
  el.push_back(6, 0);
  el.push_back(6, 1);
  el.push_back(6, 2);
  el.push_back(7, 8);
  el.push_back(7, 9);
  el.push_back(8, 0);
  el.push_back(8, 3);
  el.push_back(8, 7);
  el.push_back(9, 1);
  el.push_back(9, 4);
  el.push_back(9, 7);

  el.lexical_sort_by<0>();
  auto perm=rcm(el);
  REQUIRE(perm[0] == 8);

  el.relabel(perm);
}
