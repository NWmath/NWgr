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

#include "containers/adjacency.hpp"
#include "containers/edge_list.hpp"
#include "io/mmio.hpp"

using namespace nw::graph;
using namespace nw::util;

int main(int argc, char* argv[]) {

  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " adj.mmio [ num_points ] [ seed ]" << std::endl;
    return -1;
  }

  auto aos_a = read_mm<nw::graph::directedness::directed>(argv[1]);

  adjacency<0> A(aos_a);
  std::cout << "edge list size of directed graph is " << aos_a.size() << std::endl;
  std::cout << "compressed_sparse size of directed graph is " << A.size() << std::endl;

  for (auto&& [u, v] : aos_a)
    std::cout << u << " " << v << std::endl;

  std::cout << "===" << std::endl;

  // Should this work?
  size_t i = 0;
  for (auto&& x : A) {
    std::cout << i++ << std::endl;
    for (auto&& [u] : x) {
      std::cout << "    " << u << std::endl;
    }
  }

  // compressed_sparse<0, undirected> B(aos_a);
  // std::cout << "edge list size of undirected graph is " << aos_a.size() << std::endl;

  return 0;
}
