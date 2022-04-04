---
layout: function
title: degrees
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: build.hpp
overloads:
  "template <class Graph, class ExecutionPolicy>\nauto degrees(const Graph &, ExecutionPolicy &&)":
    arguments:
      - description: __OPTIONAL__
        name: graph
        type: const Graph &
      - description: __OPTIONAL__
        name: policy
        type: ExecutionPolicy &&
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <class Graph, class ExecutionPolicy>\nauto degrees(const Graph & graph, ExecutionPolicy && policy)"
  "template <int d_idx, class edge_list_t, class ExecutionPolicy>\nauto degrees(edge_list_t &, ExecutionPolicy &&)":
    arguments:
      - description: __OPTIONAL__
        name: el
        type: edge_list_t &
      - description: __OPTIONAL__
        name: policy
        type: ExecutionPolicy &&
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <int d_idx, class edge_list_t, class ExecutionPolicy>\nauto degrees(edge_list_t & el, ExecutionPolicy && policy)"
namespace:
  - nw
  - graph
---
