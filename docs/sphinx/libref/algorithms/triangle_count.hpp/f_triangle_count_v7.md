---
layout: function
title: triangle_count_v7
owner: Andrew Lumsdaine, Luke D'Alessandro
brief: Two-dimensional triangle counting
tags:
  - function
defined_in_file: algorithms/triangle_count.hpp
overloads:
  "template <class Graph, class OuterExecutionPolicy, class InnerExecutionPolicy>\nstd::size_t triangle_count_v7(Graph &&, OuterExecutionPolicy &&, InnerExecutionPolicy)":
    arguments:
      - description: Adjacency graph (must be upper triangular)
        name: A
        type: Graph &&
      - description: Execution policy over outer range
        name: outer
        type: OuterExecutionPolicy &&
      - description: Execution policy within inner neighbor range
        name: inner
        type: InnerExecutionPolicy
    description: This version of triangle counting is explicitly two-dimensional. It works on graphs that are upper triangular, and uses a `std::for_each` for the outer loop decomposition. <br />This version takes an outer and inner execution policy. The outer policy is used to parallelize the `std::for_each` while the inner policy is used to parallelize the `std::set_intersection`. By default the `std::for_each` is parallel while the `std::set_intersection` is sequential.
    return: The number of triangles in the graph
    signature_with_names: "template <class Graph, class OuterExecutionPolicy, class InnerExecutionPolicy>\nstd::size_t triangle_count_v7(Graph && A, OuterExecutionPolicy && outer, InnerExecutionPolicy inner)"
namespace:
  - nw
  - graph
---
