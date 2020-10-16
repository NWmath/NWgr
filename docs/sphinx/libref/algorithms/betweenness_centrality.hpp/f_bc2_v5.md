---
layout: function
title: bc2_v5
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: algorithms/betweenness_centrality.hpp
overloads:
  "template <class score_t, class accum_t, class Graph, class OuterExecutionPolicy, class InnerExecutionPolicy>\nauto bc2_v5(Graph &&, const std::vector<vertex_id_t> &, int, OuterExecutionPolicy &&, InnerExecutionPolicy &&)":
    arguments:
      - description: __OPTIONAL__
        name: graph
        type: Graph &&
      - description: __OPTIONAL__
        name: sources
        type: const std::vector<vertex_id_t> &
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
    signature_with_names: "template <class score_t, class accum_t, class Graph, class OuterExecutionPolicy, class InnerExecutionPolicy>\nauto bc2_v5(Graph && graph, const std::vector<vertex_id_t> & sources, int threads, OuterExecutionPolicy && outer_policy, InnerExecutionPolicy && inner_policy)"
namespace:
  - nw
  - graph
---
