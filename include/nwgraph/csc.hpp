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

#ifndef NW_GRAPH_CSC_HPP
#define NW_GRAPH_CSC_HPP

#include "nwgraph/adjacency.hpp"

namespace nw {
  namespace graph {

using csc_graph = adjacency<1>;

template <typename T = double>
using csc_matrix = adjacency<1, T>;

  }
}

#endif // NW_GRAPH_CSC_HPP

