---
layout: function
title: bc2_v3
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: algorithms/betweenness_centrality.hpp
overloads:
  "template <typename Graph, typename score_t, typename accum_t, class OuterExecutionPolicy, class InnerExecutionPolicy>\nauto bc2_v3(Graph &, const std::vector<vertex_id_t> &, OuterExecutionPolicy &&, InnerExecutionPolicy &&)":
    arguments:
      - description: __OPTIONAL__
        name: graph
        type: Graph &
      - description: __OPTIONAL__
        name: sources
        type: const std::vector<vertex_id_t> &
      - description: __OPTIONAL__
        name: outer_policy
        type: OuterExecutionPolicy &&
      - description: __OPTIONAL__
        name: inner_policy
        type: InnerExecutionPolicy &&
    description: __OPTIONAL__
    return: __OPTIONAL__
    signature_with_names: "template <typename Graph, typename score_t, typename accum_t, class OuterExecutionPolicy, class InnerExecutionPolicy>\nauto bc2_v3(Graph & graph, const std::vector<vertex_id_t> & sources, OuterExecutionPolicy && outer_policy, InnerExecutionPolicy && inner_policy)"
namespace:
  - nw
  - graph
---
