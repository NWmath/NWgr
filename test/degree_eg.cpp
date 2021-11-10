// 
// This file is part of NW Graph (aka GraphPack) 
// (c) Pacific Northwest National Laboratory 2018-2021 
// (c) University of Washington 2018-2021 
// 
// Licensed under terms of included LICENSE file 
// 
// Authors: 
//     Andrew Lumsdaine	
//

#include "nwgraph/edge_list.hpp"
#include "nwgraph/adjacency.hpp"
#include "karate.hpp"

int main() {

  nw::graph::adjacency<0> A(karate_d);

  for (size_t i = 0; i < A.size(); ++i) {
    std::cout << nw::graph::degree(A, i) << std::endl;
  }

  for (auto&& j : A) {
    std::cout << nw::graph::degree(j) << std::endl;    
  }

  for (auto&& j : A) {
    std::cout << nw::graph::degree(A, j) << std::endl;    
  }

}
