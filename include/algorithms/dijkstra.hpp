//
// This file is part of the Graph Standard Library (aka BGL17 aka NWGraph)
// (c) Pacific Northwest National Laboratory
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0
// International License https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#ifndef DIJKSTRA_HPP
#define DIJKSTRA_HPP

#include <algorithm>
#include <cassert>
#include <queue>
#include <tuple>
#include <vector>

#include "adaptors/bfs_edge_range.hpp"
#include "util/types.hpp"

namespace nw {
namespace graph {

//****************************************************************************
/// @todo cannot yet specify const graph.
template <typename DistanceT, typename GraphT>
std::vector<DistanceT> dijkstra(GraphT& graph, vertex_id_t source) {
  size_t N(graph.end() - graph.begin());
  assert(source < N);

  std::vector<DistanceT> distance(N, 0xDEADBEEF);
  distance[source] = 0;

  using weight_t        = DistanceT;
  using weighted_vertex = std::tuple<vertex_id_t, weight_t>;

  std::priority_queue<weighted_vertex, std::vector<weighted_vertex>, std::greater<weighted_vertex>> Q;

  for (auto&& [u, v, w] : bfs_edge_range2(graph, Q)) {
    if (distance[u] + w < distance[v]) {
      distance[v] = distance[u] + w;
      Q.push({v, distance[v]});
    }
  }

  return distance;
}

//****************************************************************************
/// @todo cannot yet specify const graph.
template <typename DistanceT, typename GraphT>
auto dijkstra_v0(GraphT& graph, vertex_id_t source) {
  size_t N(graph.end() - graph.begin());
  assert(source < N);

  std::vector<DistanceT> distance(N, std::numeric_limits<vertex_id_t>::max());
  distance[source] = 0;

  auto g                = graph.begin();
  using weight_t        = DistanceT;
  using weighted_vertex = std::tuple<vertex_id_t, weight_t>;

  std::priority_queue<weighted_vertex, std::vector<weighted_vertex>, std::greater<weighted_vertex>> Q;

  Q.push({source, distance[source]});

  while (!Q.empty()) {

    auto u = std::get<0>(Q.top());
    Q.pop();

    std::for_each(g[u].begin(), g[u].end(), [&](auto&& e) {
      auto v = std::get<0>(e);
      auto w = std::get<1>(e);
      if (distance[u] + w < distance[v]) {
        distance[v] = distance[u] + w;
        Q.push({v, distance[v]});
      }
    });
  }
  return distance;
}

}    // namespace graph
}    // namespace nw
#endif    // DIJKSTRA_HPP
