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

#ifndef DIJKSTRA_HPP
#define DIJKSTRA_HPP

#include <algorithm>
#include <cassert>
#include <queue>
#include <tuple>
#include <vector>

#include "nwgraph/graph_concepts.hpp"
#include "nwgraph/adaptors/bfs_edge_range.hpp"

namespace nw {
namespace graph {

template <typename DistanceT, adjacency_list_graph GraphT>
std::vector<DistanceT> dijkstra(const GraphT& graph, vertex_id_t<GraphT> source) {
  using vertex_id_type = vertex_id_t<GraphT>;

  size_t N(graph.end() - graph.begin());
  assert(source < N);

  std::vector<DistanceT> distance(N, 0xDEADBEEF);
  distance[source] = 0;

  using weight_t        = DistanceT;
  using weighted_vertex = std::tuple<vertex_id_type, weight_t>;

  std::priority_queue<weighted_vertex, std::vector<weighted_vertex>, std::greater<weighted_vertex>> Q;

  for (auto&& [u, v, w] : bfs_edge_range2(graph, Q)) {
    if (distance[u] + w < distance[v]) {
      distance[v] = distance[u] + w;
      Q.push({v, distance[v]});
    }
  }

  return distance;
}

template <typename DistanceT, adjacency_list_graph Graph,
          std::invocable<inner_value_t<Graph>> Weight = std::function<std::tuple_element_t<
              1, inner_value_t<Graph>>(const inner_value_t<Graph>&)>> 
auto dijkstra_v0(
          const Graph& graph, vertex_id_t<Graph> source,
          Weight weight = [](auto& e) {
            return std::get<1>(e);
          }) {
  using vertex_id_type = vertex_id_t<Graph>;

  size_t N(graph.end() - graph.begin());
  assert(source < N);

  std::vector<DistanceT> distance(N, std::numeric_limits<vertex_id_type>::max());
  distance[source] = 0;

  auto g                = graph.begin();
  using weight_t        = DistanceT;
  using weighted_vertex = std::tuple<vertex_id_type, weight_t>;

  std::priority_queue<weighted_vertex, std::vector<weighted_vertex>, std::greater<weighted_vertex>> Q;

  Q.push({source, distance[source]});

  while (!Q.empty()) {

    auto u = std::get<0>(Q.top());
    Q.pop();

    std::for_each(g[u].begin(), g[u].end(), [&](auto&& e) {
      auto v = target(graph, e);
      auto w = weight(e);
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
