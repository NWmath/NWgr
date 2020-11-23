---
layout: function
title: bc2_v5
owner: Andrew Lumsdaine
brief: Unweighted approximate betweenness centrality
tags:
  - function
defined_in_file: algorithms/betweenness_centrality.hpp
overloads:
  "template <class score_t, class accum_t, class Graph, class OuterExecutionPolicy, class InnerExecutionPolicy>\nauto bc2_v5(Graph &&, const std::vector<vertex_id_t> &, int, OuterExecutionPolicy &&, InnerExecutionPolicy &&)":
    arguments:
      - description: Adjacency graph
        name: graph
        type: Graph &&
      - description: Vector of sources
        name: sources
        type: const std::vector<vertex_id_t> &
      - description: Number of bins to store frontier
        name: threads
        type: int
      - description: Execution policy for processing frontier
        name: outer_policy
        type: OuterExecutionPolicy &&
      - description: Execution policy for processing individual neighbor lists
        name: inner_policy
        type: InnerExecutionPolicy &&
    description: Similar to the approximate betweenness centrality in bc2_v4, but extra async parallelsim over the source vertices.
    return: A vector containing the approximate betweenness centrality of every vertex
    signature_with_names: "template <class score_t, class accum_t, class Graph, class OuterExecutionPolicy, class InnerExecutionPolicy>\nauto bc2_v5(Graph && graph, const std::vector<vertex_id_t> & sources, int threads, OuterExecutionPolicy && outer_policy, InnerExecutionPolicy && inner_policy)"
namespace:
  - nw
  - graph
---
