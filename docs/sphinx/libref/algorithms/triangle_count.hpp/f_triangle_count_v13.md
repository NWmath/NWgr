---
layout: function
title: triangle_count_v13
owner: Andrew Lumsdaine, Luke D'Alessandro
brief: Two-dimensional triangle counting
tags:
  - function
defined_in_file: algorithms/triangle_count.hpp
overloads:
  "template <class Graph, class SetExecutionPolicy>\nstd::size_t triangle_count_v13(Graph &&, int, SetExecutionPolicy &&)":
    arguments:
      - description: Adjacency graph
        name: graph
        type: Graph &&
      - description: The maximum stride for the outer loop
        name: stride
        type: int
      - description: The execution policy for the set intersection
        name: set
        type: SetExecutionPolicy &&
    description: This version of triangle counting is explicitly two dimensional and uses the cyclic range adapter and a `parallel for` to process the outer dimension. <br /> This implementation is safe for both upper and lower triangular matrices, but should be more efficient for lower triangular matrices as the inner intersections will be smaller.
    return: The number of triangles in the graph
    signature_with_names: "template <class Graph, class SetExecutionPolicy>\nstd::size_t triangle_count_v13(Graph && graph, int stride, SetExecutionPolicy && set)"
namespace:
  - nw
  - graph
---
