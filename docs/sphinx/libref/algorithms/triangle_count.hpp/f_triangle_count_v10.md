---
layout: function
title: triangle_count_v10
owner: Andrew Lumsdaine, Luke D'Alessandro
brief: Two-dimensional triangle counting
tags:
  - function
defined_in_file: algorithms/triangle_count.hpp
overloads:
  "template <class Graph, class OuterExecutionPolicy, class InnerExecutionPolicy, class SetExecutionPolicy>\nstd::size_t triangle_count_v10(Graph &&, OuterExecutionPolicy &&, InnerExecutionPolicy &&, SetExecutionPolicy &&)":
    arguments:
      - description: Adjacency graph (must be upper triangular)
        name: A
        type: Graph &&
      - description: Execution policy over outer range
        name: outer
        type: OuterExecutionPolicy &&
      - description: Execution policy within inner neighbor range
        name: inner
        type: InnerExecutionPolicy &&
      - description: Set intersection execution policy
        name: set
        type: SetExecutionPolicy &&
    description: This version of triangle counting is explicitly two dimensional, uses basic `std::for_each` loop parallelism for the two loops, and supports a parallel inner set intersection. It works on either upper or lower triangular graphs.
    return: The number of triangles in the graph
    signature_with_names: "template <class Graph, class OuterExecutionPolicy, class InnerExecutionPolicy, class SetExecutionPolicy>\nstd::size_t triangle_count_v10(Graph && A, OuterExecutionPolicy && outer, InnerExecutionPolicy && inner, SetExecutionPolicy && set)"
namespace:
  - nw
  - graph
---
