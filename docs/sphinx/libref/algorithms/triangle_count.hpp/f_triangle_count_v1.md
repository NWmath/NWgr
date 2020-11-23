---
layout: function
title: triangle_count_v1
owner: Andrew Lumsdaine
brief: Two-dimensional triangle counting
tags:
  - function
defined_in_file: algorithms/triangle_count.hpp
overloads:
  "template <typename GraphT>\nsize_t triangle_count_v1(GraphT &)":
    arguments:
      - description: Adjacency graph
        name: A
        type: GraphT &
    description: Range based triangle similar to v0, but passes iterators for the first range for the set intersection to reduce unnecessary work.
    return: The number of triangles in the graph
    signature_with_names: "template <typename GraphT>\nsize_t triangle_count_v1(GraphT & A)"
namespace:
  - nw
  - graph
---
