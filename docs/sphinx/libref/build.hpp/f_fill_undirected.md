---
layout: function
title: fill_undirected
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: build.hpp
overloads:
  "template <int idx, class edge_list_t, class Int, class adjacency_t, class ExecutionPolicy>\nauto fill_undirected(edge_list_t &, Int, adjacency_t &, ExecutionPolicy &&)":
    arguments:
      - description: __OPTIONAL__
        name: el
        type: edge_list_t &
      - description: __OPTIONAL__
        name: N
        type: Int
      - description: __OPTIONAL__
        name: cs
        type: adjacency_t &
      - description: __OPTIONAL__
        name: policy
        type: ExecutionPolicy &&
    description: __OPTIONAL__
    return: __OPTIONAL__
    signature_with_names: "template <int idx, class edge_list_t, class Int, class adjacency_t, class ExecutionPolicy>\nauto fill_undirected(edge_list_t & el, Int N, adjacency_t & cs, ExecutionPolicy && policy)"
namespace:
  - nw
  - graph
---
