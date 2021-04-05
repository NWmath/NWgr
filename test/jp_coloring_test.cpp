//
// This file is part of Standard Graph Library (SGL)
// (c) Pacific Northwest National Laboratory 2018
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Jesun Sahariar Firoz
//

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <limits>
#include <list>

#include "nwgraph/algorithms/jones_plassmann_coloring.hpp"
#include "nwgraph/containers/aos.hpp"
#include "nwgraph/containers/compressed.hpp"
#include "nwgraph/io/mmio.hpp"
#include "nwgraph/util/util.hpp"

#include "common/test_header.hpp"

using namespace nw::graph;
using namespace nw::util;

typedef adjacency<0> csr_graph;

TEST_CASE("Jones-Plassmann Coloring", "[jp]") {

  /* Read the edgelist */
  auto aos_a = read_mm<directedness::undirected>(DATA_DIR "coloringData.mmio");

  swap_to_triangular<0>(aos_a, succession::predecessor);
  sort_by<1>(aos_a);
  stable_sort_by<0>(aos_a);

  /* Construct the graph */
  adjacency<0> A(aos_a);
  using adj_size_t = vertex_id_t<adjacency<0>>;

  adj_size_t N = num_vertices(A);
  std::vector<adj_size_t> colors(N, std::numeric_limits<adj_size_t>::max());

  std::vector<adj_size_t> result = {0, 1, 1, 0, 0, 0, 0, 1};
  jones_plassmann_coloring(A, colors);
  REQUIRE(colors == result);
}
