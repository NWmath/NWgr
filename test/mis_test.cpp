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
#include <cassert>
#include <iostream>

#include "graph_traits.hpp"

#include "algorithms/dag_based_mis.hpp"
#include "algorithms/mis.hpp"
#include "containers/aos.hpp"
#include "containers/compressed.hpp"
#include "io/mmio.hpp"
#include "util/util.hpp"

#include "common/test_header.hpp"

using namespace nw::graph;
using namespace nw::util;

typedef adjacency<0> csr_graph;

TEST_CASE("Maximal independent set", "[mis]") {
  /*Read the edgelist*/
  auto aos_a = read_mm<directedness::undirected>(DATA_DIR "coloringData.mmio");

  swap_to_triangular<0, decltype(aos_a), succession::predecessor>(aos_a);
  sort_by<1>(aos_a);
  stable_sort_by<0>(aos_a);

  /*Construct the graph*/
  adjacency<0> A(aos_a);

  size_t              N = A.size();
  std::vector<size_t> independentSet;
  mis_algorithm(A, independentSet);
  // for (auto v: independentSet)
  //   std::cout << v << " ";
  std::vector<size_t> result = {0, 3, 4, 5, 6};
  CHECK(independentSet == result);

  std::vector<bool> inIndependentSet(N, true);
  dag_based_mis(A, inIndependentSet);

  size_t              i = 0;
  std::vector<size_t> mis2;
  for (auto v : inIndependentSet) {
    if (v == true) {
      mis2.push_back(i);
      //     std::cout << i << " is in independent set" << std::endl;
    }
    i++;
  }
  CHECK(mis2 == result);
}
