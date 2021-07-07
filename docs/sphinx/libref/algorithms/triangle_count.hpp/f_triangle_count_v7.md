---
layout: function
title: triangle_count_v7
owner: Andrew Lumsdaine, Luke D'Alessandro
brief: Two-dimensional triangle counting
tags:
  - function
defined_in_file: algorithms/triangle_count.hpp
overloads:
  "template <class Graph, class OuterExecutionPolicy, class InnerExecutionPolicy>\nstd::size_t triangle_count_v7(const Graph &, OuterExecutionPolicy &&, InnerExecutionPolicy)":
    arguments:
      - description: __OPTIONAL__
        name: A
        type: const Graph &
      - description: __OPTIONAL__
        name: outer
        type: OuterExecutionPolicy &&
      - description: __OPTIONAL__
        name: inner
        type: InnerExecutionPolicy
    description: __OPTIONAL__
    return: __OPTIONAL__
    signature_with_names: "template <class Graph, class OuterExecutionPolicy, class InnerExecutionPolicy>\nstd::size_t triangle_count_v7(const Graph & A, OuterExecutionPolicy && outer, InnerExecutionPolicy inner)"
namespace:
  - nw
  - graph
---
