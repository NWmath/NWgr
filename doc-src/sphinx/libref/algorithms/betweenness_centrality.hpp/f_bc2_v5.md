---
layout: function
title: bc2_v5
owner: Andrew Lumsdaine
brief: Unweighted approximate betweenness centrality
tags:
  - function
defined_in_file: algorithms/betweenness_centrality.hpp
overloads:
  "template <class score_t, class accum_t, class Graph, class OuterExecutionPolicy, class InnerExecutionPolicy>\nauto bc2_v5(const Graph &, const std::vector<typename Graph::vertex_id_type> &, int, OuterExecutionPolicy &&, InnerExecutionPolicy &&)":
    arguments:
      - description: __OPTIONAL__
        name: graph
        type: const Graph &
      - description: __OPTIONAL__
        name: sources
        type: const std::vector<typename Graph::vertex_id_type> &
      - description: __OPTIONAL__
        name: threads
        type: int
      - description: __OPTIONAL__
        name: outer_policy
        type: OuterExecutionPolicy &&
      - description: __OPTIONAL__
        name: inner_policy
        type: InnerExecutionPolicy &&
    description: __OPTIONAL__
    return: __OPTIONAL__
    signature_with_names: "template <class score_t, class accum_t, class Graph, class OuterExecutionPolicy, class InnerExecutionPolicy>\nauto bc2_v5(const Graph & graph, const std::vector<typename Graph::vertex_id_type> & sources, int threads, OuterExecutionPolicy && outer_policy, InnerExecutionPolicy && inner_policy)"
namespace:
  - nw
  - graph
---
