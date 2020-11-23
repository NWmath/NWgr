//
// This file is part of Standard Graph Library (SGL)
// (c) Pacific Northwest National Laboratory 2018
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Jesun Sahariar Firoz
//

/**/

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <limits>
#include <list>

#include "algorithms/connected_components.hpp"
#include "containers/aos.hpp"
#include "containers/compressed.hpp"
#include "io/mmio.hpp"
#include "util/util.hpp"

#include "common/test_header.hpp"

using namespace nw::graph;
using namespace nw::util;

typedef compressed_sparse<0, directed> csr_graph;

TEST_CASE("connected component", "[cc]") {
  /*Read the edgelist*/
  auto aos_a = read_mm<directed>(DATA_DIR "coloringData.mmio");
  // aos_a.triangularize<predecessor>();
  // aos_a.sort_by<1>();
  // aos_a.stable_sort_by<0>();

  /*Construct the graph*/
  compressed_sparse<0, directed> A(aos_a);

  size_t              N = A.size();
  std::vector<vertex_id_t> component_ids(N, std::numeric_limits<vertex_id_t>::max());


  SECTION("v1 cc_push") {
    component_ids = ccv1(A);
    for (auto component_id : component_ids) {
      //std::cout << component_id << std::endl;
      REQUIRE(component_id == 0);
    }
  }

  SECTION("v2 cc_pull") {
    component_ids = compute_connected_components_v2(A);
    for (auto component_id : component_ids) {
      //std::cout << component_id << std::endl;
      REQUIRE(component_id == 0);
    }
  }
  SECTION("v5 cc_pull + subgraph sampling") {
    component_ids = ccv5(A);
    for (auto component_id : component_ids) {
      //std::cout << component_id << std::endl;
      REQUIRE(component_id == 0);
    }
  }
  SECTION("v6 sv") {
    component_ids = sv_v6(A);
    for (auto component_id : component_ids) {
      //std::cout << component_id << std::endl;
      REQUIRE(component_id == 0);
    }
  }
  SECTION("v8 sv") {
    component_ids = sv_v8(A);
    for (auto component_id : component_ids) {
      //std::cout << component_id << std::endl;
      REQUIRE(component_id == 0);
    }
  }
}
