---
layout: function
title: triangle_count_v10
owner: Andrew Lumsdaine, Luke D'Alessandro
brief: Two-dimensional triangle counting
tags:
  - function
defined_in_file: algorithms/triangle_count.hpp
overloads:
  "template <class Graph, class OuterExecutionPolicy, class InnerExecutionPolicy, class SetExecutionPolicy>\nstd::size_t triangle_count_v10(const Graph &, OuterExecutionPolicy &&, InnerExecutionPolicy &&, SetExecutionPolicy &&)":
    arguments:
      - description: __OPTIONAL__
        name: A
        type: const Graph &
      - description: __OPTIONAL__
        name: outer
        type: OuterExecutionPolicy &&
      - description: __OPTIONAL__
        name: inner
        type: InnerExecutionPolicy &&
      - description: __OPTIONAL__
        name: set
        type: SetExecutionPolicy &&
    description: __OPTIONAL__
    return: __OPTIONAL__
    signature_with_names: "template <class Graph, class OuterExecutionPolicy, class InnerExecutionPolicy, class SetExecutionPolicy>\nstd::size_t triangle_count_v10(const Graph & A, OuterExecutionPolicy && outer, InnerExecutionPolicy && inner, SetExecutionPolicy && set)"
namespace:
  - nw
  - graph
---
