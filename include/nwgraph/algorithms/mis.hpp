// 
// This file is part of NW Graph (aka GraphPack) 
// (c) Pacific Northwest National Laboratory 2018-2021 
// (c) University of Washington 2018-2021 
// 
// Licensed under terms of include LICENSE file 
// 
// Authors: 
//     Andrew Lumsdaine	
//     Kevin Deweese	
//

#ifndef NW_GRAPH_MIS_HPP
#define NW_GRAPH_MIS_HPP

#include "nwgraph/adaptors/bfs_range.hpp"

namespace nw {
namespace graph {

template <typename Graph>
void mis_algorithm(Graph A, std::vector<size_t>& mis) {
  size_t            N = A.size();
  std::vector<bool> removedVertices(N);
  for (size_t vtx = 0; vtx < N; vtx++) {
    if (!removedVertices[vtx]) {
      mis.push_back(vtx);
      for (auto ite = A.begin()[vtx].begin(); ite != A.begin()[vtx].end(); ++ite) {
        // Explore neighbors
        auto u             = std::get<0>(*ite);
        removedVertices[u] = true;
      }
    }
  }
}
}    // namespace graph
}    // namespace nw
#endif    // NW_GRAPH_MIS_HPP
