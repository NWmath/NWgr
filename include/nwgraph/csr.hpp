// 
// This file is part of NW Graph (aka GraphPack) 
// (c) Pacific Northwest National Laboratory 2018-2021 
// (c) University of Washington 2018-2021 
// 
// Licensed under terms of include LICENSE file 
// 
// Authors: 
//     Andrew Lumsdaine	
//

#ifndef NW_GRAPH_CSR_HPP
#define NW_GRAPH_CSR_HPP

#include "nwgraph/adjacency.hpp"

namespace nw {
  namespace graph {

using csr_graph = adjacency<0>;

template <typename T = double>
using csr_matrix = adjacency<0, T>;

  }
}

#endif // NW_GRAPH_CSR_HPP
