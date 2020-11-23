---
layout: function
title: bc2_v2
owner: Andrew Lumsdaine
brief: Unweighted approximate betweenness centrality
tags:
  - function
defined_in_file: algorithms/betweenness_centrality.hpp
overloads:
  "template <typename Graph, typename score_t, typename accum_t, class ExecutionPolicy>\nauto bc2_v2(Graph &, const std::vector<vertex_id_t> &, ExecutionPolicy &&)":
    arguments:
      - description: Adjacency graph
        name: graph
        type: Graph &
      - description: Vector of sources
        name: sources
        type: const std::vector<vertex_id_t> &
      - description: Execution policy for processing frontier
        name: policy
        type: ExecutionPolicy &&
    description: Similar to bc2_v1, with an outer execution policy for processing the frontier in parallel.
    return: A vector containing the approximate betweenness centrality of every vertex
    signature_with_names: "template <typename Graph, typename score_t, typename accum_t, class ExecutionPolicy>\nauto bc2_v2(Graph & graph, const std::vector<vertex_id_t> & sources, ExecutionPolicy && policy)"
namespace:
  - nw
  - graph
---
