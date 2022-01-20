//
// This file is part of Standard Graph Library (SGL)
// (c) Pacific Northwest National Laboratory 2018
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#include <concepts>

#include "containers/aolos.hpp"
#include "nwgraph/containers/compressed.hpp"
#include "nwgraph/edge_list.hpp"
#include "nwgraph/vovos.hpp"
#include "nwgraph/graph_concepts.hpp"

#include "common/abstract_test.hpp"

/*template<typename GraphT>
requires AdjacencyGraph<GraphT>
void test_func(GraphT& G) {
  
}*/

TEST_CASE("test", "[test]") {

  adj_list<> A(5);
  A.push_back(0, 1);
  A.push_back(1, 2);
  A.push_back(0, 2);
  A.push_back(1, 3);
  A.push_back(1, 4);
  std::ranges::begin(A);
  for (auto&& out : A) {
    std::cout << "outer" << std::endl;
    for (auto&& in : out) {
      std::cout << "in" << std::endl;
    }
  }
  std::ranges::for_each(
      A, [](auto&& out) { std::ranges::for_each(out, [](auto&& in) { std::cout << "edge to " << std::get<0>(in) << std::endl; }); });
  //test_func(A);
}
