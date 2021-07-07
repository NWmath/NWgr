---
layout: function
title: bc2_v2
owner: Andrew Lumsdaine
brief: Unweighted approximate betweenness centrality
tags:
  - function
defined_in_file: algorithms/betweenness_centrality.hpp
overloads:
  "template <typename Graph, typename score_t, typename accum_t, class ExecutionPolicy>\nauto bc2_v2(const Graph &, const std::vector<typename Graph::vertex_id_type> &, ExecutionPolicy &&)":
    arguments:
      - description: __OPTIONAL__
        name: graph
        type: const Graph &
      - description: __OPTIONAL__
        name: sources
        type: const std::vector<typename Graph::vertex_id_type> &
      - description: __OPTIONAL__
        name: policy
        type: ExecutionPolicy &&
    description: __OPTIONAL__
    return: __OPTIONAL__
    signature_with_names: "template <typename Graph, typename score_t, typename accum_t, class ExecutionPolicy>\nauto bc2_v2(const Graph & graph, const std::vector<typename Graph::vertex_id_type> & sources, ExecutionPolicy && policy)"
namespace:
  - nw
  - graph
---
