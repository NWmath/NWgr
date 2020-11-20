//
// This file is part of Standard Graph Library (SGL)
// (c) Pacific Northwest National Laboratory 2018
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0
// International License https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Jesun Sahariar Firoz
//

#ifndef NW_GRAPH_MIS_HPP
#define NW_GRAPH_MIS_HPP

#include "adaptors/bfs_range.hpp"

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
