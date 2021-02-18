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
size_t jaccard_similarity_v0(const GraphT& A) {
  size_t ctr = 0;
  auto   first     = A.begin();
  auto   last      = A.end();


  for (auto G = first; first != last; ++first) {
    for (auto v = (*first).begin(); v != (*first).end(); ++v) {
      auto numer = nw::graph::intersection_size(*first, G[std::get<0>(*v)]);
      auto denom = degree(*first) + degree(G[std::get<0>(*v)]) - numer;
      double rat   = ((double)numer) / ((double)denom);
      std::get<2>(*v) = rat;
      ++ctr;
    }
  }
  return ctr;
}


template <typename GraphT>
size_t jaccard_similarity_v1(const GraphT& A) {
  size_t ctr = 0;
  auto   first     = A.begin();
  auto   last      = A.end();

  for (auto G = first; first != last; ++first) {
    for (auto v = (*first).begin(); v != (*first).end(); ++v) {
      auto numer = nw::graph::intersection_size(v (*first).end(), G[std::get<0>(*v)]);
      auto denom = degree(*first) + degree(G[std::get<0>(*v)]) - numer;
      double rat   = ((double)numer) / ((double)denom);
      std::get<2>(*v) = rat;
      ++ctr;
    }
  }
  return ctr;
}

template <typename GraphT>
auto jaccard_similarity_v2(GraphT& graph) {

  size_t ctr = 0;
  auto deg = degrees(graph);
  for (auto&& [u, v, w] : make_edge_range<0, 1, 2>(graph)) {
    auto   numer = intersection_size(graph[u], graph[v]);
    auto   denom = deg[u] + deg[v] - numer;
    double rat   = ((double)numer) / ((double)denom);
    w            = rat;
    ++ctr;
  }
  return ctr;
}



}    // namespace graph
}    // namespace nw

#endif    // NW_GRAPH_JACCARD_HPP
