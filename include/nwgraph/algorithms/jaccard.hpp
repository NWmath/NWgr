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

#include <atomic>
#include <future>
#include <thread>
#include <tuple>
#include <vector>

#include "nwgraph/graph_concepts.hpp"

#include "nwgraph/adaptors/cyclic_range_adaptor.hpp"
#include "nwgraph/adaptors/edge_range.hpp"
#include "nwgraph/util/intersection_size.hpp"
#include "nwgraph/util/parallel_for.hpp"
#include "nwgraph/util/timer.hpp"
#include "nwgraph/util/util.hpp"

namespace nw {
namespace graph {

template <adjacency_list_graph GraphT, typename Weight>
size_t jaccard_similarity_v0(GraphT& G, Weight weight) {
  size_t ctr = 0;

  for (size_t u = 0; u < num_vertices(G); ++u) {
    for (auto&& e : G[u]) {
      auto v = target(G, e);
      if (u < v) {
        auto numer = nw::graph::intersection_size(G[u], G[v]);
        auto denom = degree(G[u]) + degree(G[v]) - numer;
        double rat = ((double)numer) / ((double)denom);
        weight(e) = rat;
        ++ctr;
      }
    }
  }

  return ctr;
}

}    // namespace graph
}    // namespace nw

#endif    // NW_GRAPH_JACCARD_HPP
