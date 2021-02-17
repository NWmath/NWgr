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

#ifndef NW_GRAPH_JACCARD_HPP
#define NW_GRAPH_JACCARD_HPP

#include "nwgraph/adaptors/cyclic_range_adapter.hpp"
#include "nwgraph/util/parallel_for.hpp"
#include <atomic>
#include <future>
#include <thread>

#include "nwgraph/adaptors/edge_range.hpp"
#include "nwgraph/util/intersection_size.hpp"
#include "util/timer.hpp"
#include "nwgraph/util/util.hpp"

#include <tuple>
#include <vector>

namespace nw {
namespace graph {

template <typename GraphT>
auto jaccard_v0(GraphT& graph) {

  auto deg = degrees(graph);
  for (auto&& [u, v, w] : make_edge_range<0, 1, 2>(graph)) {
    auto   numer = intersection_size(graph[u], graph[v]);
    auto   denom = deg[u] + deg[v] - numer;
    double rat   = ((double)numer) / ((double)denom);
    w            = rat;
  }
}

}    // namespace graph
}    // namespace nw

#endif    // NW_GRAPH_JACCARD_HPP
