//
// This file is part of Standard Graph Library (SGL)
// (c) Pacific Northwest National Laboratory 2018
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0
// International License https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#ifndef NW_GRAPH_JACCARD_HPP
#define NW_GRAPH_JACCARD_HPP

#include "adaptors/cyclic_range_adapter.hpp"
#include "util/parallel_for.hpp"
#include <atomic>
#include <future>
#include <thread>

#include "adaptors/edge_range.hpp"
#include "util/intersection_size.hpp"
#include "util/timer.hpp"
#include "util/util.hpp"

#include <tuple>
#include <vector>

namespace nw {
namespace graph {

template <typename GraphT>
auto jaccard_v0(GraphT& graph) {

  auto deg = degrees(graph);
  for (auto&& [u, v, w] : make_edge_range<0,1,2>(graph)) {
    auto   numer = intersection_size(graph[u], graph[v]);
    auto   denom = deg[u] + deg[v] - numer;
    double rat   = ((double)numer) / ((double)denom);
    w = rat;
  }
}

}    // namespace graph
}    // namespace nw

#endif    // NW_GRAPH_JACCARD_HPP