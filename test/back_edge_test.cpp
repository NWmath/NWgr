//
// This file is part of BGL17 (aka NWGraph aka GraphPack aka the Graph Standard Library)
// (c) Pacific Northwest National Laboratory 2018
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Kevin Deweese
//

#include "adaptors/back_edge_range.hpp"
#include "containers/compressed.hpp"
#include "containers/edge_list.hpp"
#include "adaptors/filtered_bfs_range.hpp"
#include "adaptors/reverse.hpp"

#include "common/test_header.hpp"

using namespace nw::graph;
using namespace nw::util;

TEST_CASE("back edge", "[back edge]") {

  edge_list<directed, double, double> E_list(6);
  E_list.push_back(0, 1, 1.0, 0);
  E_list.push_back(1, 2, 2.0, 0);
  E_list.push_back(2, 3, 3.0, 0);
  E_list.push_back(3, 2, 4.0, 0);
  E_list.push_back(3, 4, 5.0, 0);
  E_list.push_back(4, 3, 6.0, 0);
  E_list.push_back(5, 4, 2.0, 0);

  adjacency<0, double, double> A(E_list);

  back_edge_range back(A);

  auto G = back.begin();
  for (auto outer = back.begin(); outer != back.end(); ++outer) {
    for (auto inner = G[outer - G].begin(); inner != G[outer - G].end(); ++inner) {
      REQUIRE(outer - back.begin() == std::get<0>(back.get_back_edge(outer - back.begin(), inner)));
    }
  }
}
