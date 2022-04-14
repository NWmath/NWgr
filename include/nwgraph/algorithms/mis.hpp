/**
 * @file mis.hpp
 *
 * @copyright SPDX-FileCopyrightText: 2022 Batelle Memorial Institute
 * @copyright SPDX-FileCopyrightText: 2022 University of Washington
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * @authors
 *   Andrew Lumsdaine
 *   Tony Liu
 *
 */

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

#include "nwgraph/graph_concepts.hpp"
#include "nwgraph/adaptors/bfs_range.hpp"

namespace nw {
namespace graph {

template <adjacency_list_graph Graph>
void mis_algorithm(const Graph& A, std::vector<size_t>& mis) {
  using vertex_id_type = vertex_id_t<Graph>;
  size_t            N = A.size();
  std::vector<bool> removedVertices(N);
  for (vertex_id_type vtx = 0; vtx < N; vtx++) {
    if (!removedVertices[vtx]) {
      mis.push_back(vtx);
      for (auto ite = A[vtx].begin(); ite != A[vtx].end(); ++ite) {
        // Explore neighbors
        auto u = target(A, *ite);
        removedVertices[u] = true;
      }
    }
  }
}
}    // namespace graph
}    // namespace nw
#endif    // NW_GRAPH_MIS_HPP
