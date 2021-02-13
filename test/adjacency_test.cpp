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

#include "nwgraph/adaptors/edge_range.hpp"
#include "nwgraph/adjacency.hpp"
#include "nwgraph/edge_list.hpp"
#include "nwgraph/io/mmio.hpp"

#include "common/test_header.hpp"

using namespace nw::graph;
using namespace nw::util;

TEST_CASE("adjacency", "[adjacency]") {
  size_t n_vtx = 5;

  edge_list<directedness::directed, double> A_list(n_vtx);
  A_list.push_back(0, 1, 1);
  A_list.push_back(1, 2, 2);
  A_list.push_back(2, 3, 3);
  A_list.push_back(3, 4, 4);

  adjacency<0, double> A(A_list);

  for (auto&& [u, v, w] : make_edge_range<0>(A)) {
    std::cout << "edge " << u << " to " << v << " has weight " << w << std::endl;
  }

  edge_list<directedness::undirected, double> B_list(n_vtx);
  B_list.push_back(0, 1, 10);
  B_list.push_back(1, 2, 20);
  B_list.push_back(2, 3, 30);
  B_list.push_back(3, 4, 40);

  adjacency<0, double> B(B_list);

  B.stream_indices();

  for (auto&& [u, v, w] : make_edge_range<0>(B)) {
    std::cout << "edge " << u << " to " << v << " has weight " << w << std::endl;
  }
}
