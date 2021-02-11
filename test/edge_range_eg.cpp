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
#include <queue>

#include "adaptors/edge_range.hpp"
#include "containers/adjacency.hpp"
#include "containers/edge_list.hpp"
#include "io/mmio.hpp"

using namespace nw::graph;
using namespace nw::util;

int main(int argc, char* argv[]) {

  edge_list<nw::graph::directedness::directed> a{{0, 3}, {1, 1}, {2, 4}, {3, 1}, {4, 5}, {3, 9}, {2, 2}, {1, 7}};

  auto adj = adjacency<0>(a);

  edge_list<nw::graph::directedness::directed> b;
  auto                                         edges = make_edge_range(adj);
  for (auto&& e : edges) {
    b.push_back(e);
  }
  b.close_for_push_back();

  std::sort(a.begin(), a.end());
  std::sort(b.begin(), b.end());

  if (a == b) {
    std::cout << "pass" << std::endl;
  } else {
    std::cout << "fail" << std::endl;
  }

  edge_list<nw::graph::directedness::directed, double> c{{0, 3, 8.6}, {1, 1, 7.5}, {2, 4, 3.0}, {3, 1, 99},
                                                         {4, 5, 3.1}, {3, 9, .04}, {2, 2, 1.5}, {1, 7, 9}};

  auto d = adjacency<0, double>(c);

  auto f = make_edge_range(d);
  for (auto&& [u, v] : f) {
    ;
  }

  auto g = make_edge_range<0>(d);
  for (auto&& [u, v, w] : g) {
    ;
  }

  for (auto&& [u, v, w] : make_edge_range<0>(d)) {
    ;
  }

  for (auto&& [u, v, w] : make_edge_range<0>(d)) {
    ;
  }

  return 0;
}
