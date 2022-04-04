---
layout: function
title: triangle_count_edgerange_cyclic
owner: Andrew Lumsdaine, Luke D'Alessandro
brief: One dimensional triangle counting with a cyclic edge range
tags:
  - function
defined_in_file: algorithms/triangle_count.hpp
overloads:
  "template <class Graph, class SetExecutionPolicy>\nstd::size_t triangle_count_edgerange_cyclic(Graph &&, int, SetExecutionPolicy &&)":
    arguments:
      - description: Adjacency graph
        name: graph
        type: Graph &&
      - description: The maximum stride for the cyclic outer loop
        name: stride
        type: int
      - description: The execution policy for the set intersection
        name: set
        type: SetExecutionPolicy &&
    description: This version of triangle counting assumes that the graph supports direct edge enumeration with an edge range that has random-access iterators and supports tbb's split interface.
    return: The number of triangles in the graph
    signature_with_names: "template <class Graph, class SetExecutionPolicy>\nstd::size_t triangle_count_edgerange_cyclic(Graph && graph, int stride, SetExecutionPolicy && set)"
namespace:
  - nw
  - graph
---
