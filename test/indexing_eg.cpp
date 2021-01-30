//
// This file is part of BGL17 (aka NWGraph aka GraphPack aka the Graph Standard Library)
// (c) Pacific Northwest National Laboratory 2018
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#include <iostream>

#include "adaptors/edge_range.hpp"
#include "containers/edge_list.hpp"
#include "util/print_types.hpp"

using namespace nw::graph;
using namespace nw::util;

template <directedness dir>
void do_test_0() {

  edge_list<dir> E_list{{3, 4}, {3, 6}, {4, 6}};

  std::cout << "E_list" << std::endl;
  for (auto y : E_list) {
    std::cout << std::get<0>(y) << " " << std::get<1>(y) << std::endl;
  }
  std::cout << "A<0>" << std::endl;
  adjacency<0> A(E_list);
  for (auto outer = A.begin(); outer != A.end(); ++outer) {
    for (auto inner = (*outer).begin(); inner != (*outer).end(); ++inner) {
      std::cout << (outer)-A.begin() << " " << std::get<0>(*inner) << std::endl;
    }
  }

  std::cout << "A<1>" << std::endl;
  adjacency<1> A2(E_list);
  for (auto outer = A2.begin(); outer != A2.end(); ++outer) {
    for (auto inner = (*outer).begin(); inner != (*outer).end(); ++inner) {
      std::cout << (outer)-A2.begin() << " " << std::get<0>(*inner) << std::endl;
    }
  }

  std::cout << "plain edge range (A)" << std::endl;
  for (auto [i, j] : edge_range(A)) {
    std::cout << i << " " << j << std::endl;
  }

  std::cout << "plain edge range (A2)" << std::endl;
  for (auto [i, j] : edge_range(A2)) {
    std::cout << i << " " << j << std::endl;
  }
}

template <directedness dir>
void do_test_1() {

  edge_list<dir, float> E_list{{3, 4, 8.6}, {3, 6, 7.5}, {4, 6, 3.09}};

  std::cout << "E_list" << std::endl;
  for (auto y : E_list) {
    std::cout << std::get<0>(y) << " " << std::get<1>(y) << " " << std::get<2>(y) << std::endl;
  }
  std::cout << "A<0>" << std::endl;
  adjacency<0, float> A(E_list);
  for (auto outer = A.begin(); outer != A.end(); ++outer) {
    for (auto inner = (*outer).begin(); inner != (*outer).end(); ++inner) {
      std::cout << (outer)-A.begin() << " " << std::get<0>(*inner) << " " << std::get<1>(*inner) << std::endl;
    }
  }

  std::cout << "A<1>" << std::endl;
  adjacency<1, float> A2(E_list);
  for (auto outer = A2.begin(); outer != A2.end(); ++outer) {
    for (auto inner = (*outer).begin(); inner != (*outer).end(); ++inner) {
      std::cout << (outer)-A2.begin() << " " << std::get<0>(*inner) << " " << std::get<1>(*inner) << std::endl;
    }
  }

  std::cout << "spmv range (A) v0" << std::endl;
  for (auto&& i : make_edge_range<0>(A)) {
    std::cout << std::get<0>(i) << " " << std::get<1>(i) << " " << std::get<2>(i) << std::endl;
  }

  std::cout << "spmv range (A)" << std::endl;
  for (auto&& [i, j, v] : make_edge_range<0>(A)) {
    std::cout << i << " " << j << " " << v << std::endl;
  }
  std::cout << "spmv range (A2)" << std::endl;

  for (auto [i, j, v] : make_edge_range<0>(A2)) {
    std::cout << i << " " << j << " " << v << std::endl;
  }

  std::cout << "edge range (A) v0" << std::endl;
  for (auto&& [i, j, v] : A.edges<0>()) {
    std::cout << i << " " << j << " " << v << std::endl;
  }
}

int main() {

  std::cout << "Unirected" << std::endl;
  do_test_0<undirected>();

  std::cout << "Directed" << std::endl;
  do_test_0<directed>();

  std::cout << "Unirected" << std::endl;
  do_test_1<undirected>();

  std::cout << "Directed" << std::endl;
  do_test_1<directed>();

  edge_list<undirected> E_list{{3, 4}, {3, 6}, {4, 6}, {2, 3}, {3, 0}, {4, 5}, {0, 2}, {1, 5}, {4, 1}};

  std::cout << "E_list" << std::endl;
  for (auto y : E_list) {
    std::cout << std::get<0>(y) << " " << std::get<1>(y) << std::endl;
  }
  std::cout << "A<0>" << std::endl;
  adjacency<0> A(E_list);

  for (auto outer = A.begin(); outer != A.end(); ++outer) {
    for (auto inner = (*outer).begin(); inner != (*outer).end(); ++inner) {
      std::cout << (outer)-A.begin() << " " << std::get<0>(*inner) << std::endl;
    }
  }

  std::cout << (*(A.end())).begin() - (*(A.begin())).begin() << std::endl;
  edge_list<directed> D_list{{3, 4}, {3, 6}, {4, 6}, {2, 3}, {3, 0}, {4, 5}, {0, 2}, {1, 5}, {4, 1}};

  adjacency<0> B(D_list);
  std::cout << (*(B.end())).begin() - (*(B.begin())).begin() << std::endl;

  auto bb = (*(A.begin())).begin();
  for (auto u : A) {
    for (auto v = u.begin(); v != u.end(); ++v) {

      std::cout << v - bb << std::endl;
    }
  }

  // This should not work
  auto cc = (*(A.end())).begin();
  for (auto u : A) {
    for (auto v = u.begin(); v != u.end(); ++v) {

      std::cout << cc - v << std::endl;
    }
  }

  return 0;
}
