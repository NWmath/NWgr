//
// This file is part of BGL17 (aka NWGraph aka GraphPack aka the Graph Standard Library)
// (c) Pacific Northwest National Laboratory 2018
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Kevin Deweese
//

#include "common/test_header.hpp"
#include <algorithm>
#include <back_edge_range.hpp>
#include <algorithms/boykov_kolmogorov.hpp>
#include <compressed.hpp>
#include <edge_list.hpp>
#include <io/mmio.hpp>
#include <vector>

using namespace nw::graph;
using namespace nw::util;

TEST_CASE("boykov-kolmogorov max flow", "[bkmf]") {

  auto aos_a = read_mm<directed, double>(DATA_DIR "bktest1.mtx");

  size_t n_vtx = aos_a.size();

  edge_list<directed, double, double*> A_list(n_vtx);
  for (auto y : aos_a) {
    A_list.push_back(std::get<0>(y), std::get<1>(y), std::get<2>(y), NULL);
  }

  adjacency<0, double, double*> A(A_list);

  std::vector<double> capacities(n_vtx);

  for (auto it = capacities.begin(); it != capacities.end(); ++it) {
    *it = 1;
  }
  capacities[1] = -1;
  capacities[3] = -1;
  capacities[7] = -1;
  back_edge_range              back(A);
  auto result = bk_maxflow(back, capacities);
  REQUIRE(std::get<0>(result) == 0);
}
