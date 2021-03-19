---
layout: function
title: triangle_count_v12
owner: Andrew Lumdaine, Luke D'Alessandro
brief: Two-dimensional triangle counting
tags:
  - function
defined_in_file: algorithms/triangle_count.hpp
overloads:
  "template <class Graph, class SetExecutionPolicy>\nstd::size_t triangle_count_v12(const Graph &, int, SetExecutionPolicy &&)":
    arguments:
      - description: __OPTIONAL__
        name: graph
        type: const Graph &
      - description: __OPTIONAL__
        name: stride
        type: int
      - description: __OPTIONAL__
        name: set
        type: SetExecutionPolicy &&
    description: __OPTIONAL__
    return: __OPTIONAL__
    signature_with_names: "template <class Graph, class SetExecutionPolicy>\nstd::size_t triangle_count_v12(const Graph & graph, int stride, SetExecutionPolicy && set)"
namespace:
  - nw
  - graph
---
