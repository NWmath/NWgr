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

#include <iostream>

#include "nwgraph/graph_traits.hpp"
#include "nwgraph/access.hpp"
#include "nwgraph/adaptors/bfs_range.hpp"

namespace nw {
namespace graph {

template <typename Graph>
void mis_algorithm(Graph A, std::vector<vertex_id_t<Graph>>& mis) {
  using vertex_id_type = vertex_id_t<Graph>;
  vertex_id_type            N = num_vertices(A);
  std::vector<bool> removedVertices(N, false);
  for (vertex_id_type vtx = 0; vtx < N; vtx++) {
    if (!removedVertices[vtx]) {
      mis.push_back(vtx);
      for (auto&& [u] : A[vtx]) {
        // Explore neighbors
        removedVertices[u] = true;
      }
    }
  }
}
}    // namespace graph
}    // namespace nw
#endif    // NW_GRAPH_MIS_HPP
