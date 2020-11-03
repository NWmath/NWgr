---
layout: function
title: triangle_count_v14
owner: Andrew Lumsdaine, Luke D'Alessandro
brief: One-dimensional triangle counting
tags:
  - function
defined_in_file: algorithms/triangle_count.hpp
overloads:
  "template <class Graph, class SetExecutionPolicy>\nstd::size_t triangle_count_v14(Graph &&, SetExecutionPolicy &&)":
    arguments:
      - description: Adjacency graph
        name: graph
        type: Graph &&
      - description: The execution policy for the set intersection
        name: set
        type: SetExecutionPolicy &&
    description: This version of triangle counting uses a one dimensional parallel-for loop over the edge range. This is identical to the the GAP verification code, but in parallel. <br /> This version produces the correct result for both upper and lower triangular matrices, but should be somewhat more efficient for lower triangular orders as the set intersections should be more efficient.
    return: The number of triangles in the graph
    signature_with_names: "template <class Graph, class SetExecutionPolicy>\nstd::size_t triangle_count_v14(Graph && graph, SetExecutionPolicy && set)"
namespace:
  - nw
  - graph
---
