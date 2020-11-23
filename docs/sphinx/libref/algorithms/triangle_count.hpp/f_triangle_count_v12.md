---
layout: function
title: triangle_count_v12
owner: Andrew Lumdaine, Luke D'Alessandro
brief: Two-dimensional triangle counting
tags:
  - function
defined_in_file: algorithms/triangle_count.hpp
overloads:
  "template <class Graph, class SetExecutionPolicy>\nstd::size_t triangle_count_v12(Graph &&, int, SetExecutionPolicy &&)":
    arguments:
      - description: Adjacency graph (must be upper-triangular)
        name: graph
        type: Graph &&
      - description: The maximum stride for the cyclic outer loop
        name: stride
        type: int
      - description: The execution policy for the set intersection
        name: set
        type: SetExecutionPolicy &&
    description: This version of triangle counting is explicitly two dimensional, is optimized (and only correct for) an upper-triangular graph, and uses the cyclic range adapter and a `parallel for` to process the outer dimension.
    return: The number of triangles in the graph
    signature_with_names: "template <class Graph, class SetExecutionPolicy>\nstd::size_t triangle_count_v12(Graph && graph, int stride, SetExecutionPolicy && set)"
namespace:
  - nw
  - graph
---
