---
layout: function
title: bc2_v3
owner: Andrew Lumsdaine
brief: Unweighted approximate betweenness centrality
tags:
  - function
defined_in_file: algorithms/betweenness_centrality.hpp
overloads:
  "template <typename Graph, typename score_t, typename accum_t, class OuterExecutionPolicy, class InnerExecutionPolicy>\nauto bc2_v3(Graph &, const std::vector<vertex_id_t> &, OuterExecutionPolicy &&, InnerExecutionPolicy &&)":
    arguments:
      - description: Adjacency graph
        name: graph
        type: Graph &
      - description: Vector of sources
        name: sources
        type: const std::vector<vertex_id_t> &
      - description: Execution policy for processing frontier
        name: outer_policy
        type: OuterExecutionPolicy &&
      - description: Execution policy for processing individual neighbor lists
        name: inner_policy
        type: InnerExecutionPolicy &&
    description: Similar to bc2_v2, with an optional execution policy for processing neighbor lists in parallel.
    return: A vector containing the approximate betweenness centrality of every vertex
    signature_with_names: "template <typename Graph, typename score_t, typename accum_t, class OuterExecutionPolicy, class InnerExecutionPolicy>\nauto bc2_v3(Graph & graph, const std::vector<vertex_id_t> & sources, OuterExecutionPolicy && outer_policy, InnerExecutionPolicy && inner_policy)"
namespace:
  - nw
  - graph
---
