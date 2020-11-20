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

#include "algorithms/jones_plassmann_coloring.hpp"
#include "containers/aos.hpp"
#include "containers/compressed.hpp"
#include "io/mmio.hpp"
#include "util/util.hpp"

#include "common/test_header.hpp"

using namespace nw::graph;
using namespace nw::util;

typedef compressed_sparse<0> csr_graph;

TEST_CASE("Jones-Plassmann Coloring", "[jp]") {

  /*Read the edgelist*/
  auto aos_a = read_mm<undirected>(DATA_DIR "coloringData.mmio");
  aos_a.swap_to_triangular<0, predecessor>();
  aos_a.sort_by<1>();
  aos_a.stable_sort_by<0>();

  /*Construct the graph*/
  compressed_sparse<0, undirected> A(aos_a);

  size_t              N = A.size();
  std::vector<size_t> colors(N, std::numeric_limits<std::uint32_t>::max());

  std::vector<size_t> result = {0, 1, 1, 0, 0, 0, 0, 1};
  jones_plassmann_coloring(A, colors);
  REQUIRE(colors == result);
}
