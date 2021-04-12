//
// This file is part of BGL17 (aka NWGraph aka GraphPack aka the Graph Standard Library)
// (c) Pacific Northwest National Laboratory 2018
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine, Xu Tony Liu
//

#include <iostream>

#include "nwgraph/adaptors/neighbor_range.hpp"
#include "nwgraph/containers/compressed.hpp"
#include "nwgraph/edge_list.hpp"
#include "nwgraph/io/mmio.hpp"

using namespace nw::graph;
using namespace nw::util;

int main(int argc, char* argv[]) {

  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " adj.mmio " << std::endl;
    return -1;
  }

  auto         aos_a = read_mm<directedness::undirected>(argv[1]);
  adjacency<0> A(aos_a);

  for (auto&& [u, neighbors] : neighbor_range(A)) {
    std::cout << u << ": ";
    for (auto && [v] : neighbors)
        std::cout << v << " ";
    std::cout << std::endl;
  }
  
  
  auto neighborhoods = neighbor_range(A);
  std::for_each(std::execution::seq, neighborhoods.begin(), neighborhoods.end(), [&](auto&& x) {
    auto&& [u, neighbors] = x;
    std::cout << u << ": ";
    for (auto && [v] : neighbors)
        std::cout << v << " ";
    std::cout << std::endl;     
  });
    
  return 0;
}