---
layout: function
title: bc2_v1
owner: Andrew Lumsdaine
brief: Unweighted approximate betweenness centrality
tags:
  - function
defined_in_file: algorithms/betweenness_centrality.hpp
overloads:
  "template <typename Graph, typename score_t, typename accum_t>\nauto bc2_v1(Graph &, const std::vector<vertex_id_t>)":
    arguments:
      - description: Adjacency graph
        name: graph
        type: Graph &
      - description: Vector of sources
        name: sources
        type: const std::vector<vertex_id_t>
    description: Similar to bc2_v0, but uses atomic operations for setting depth. Used to test the atomic operations needed for the following parallel implementations.
    return: A vector containing the approximate betweenness centrality of every vertex
    signature_with_names: "template <typename Graph, typename score_t, typename accum_t>\nauto bc2_v1(Graph & graph, const std::vector<vertex_id_t> sources)"
namespace:
  - nw
  - graph
---
