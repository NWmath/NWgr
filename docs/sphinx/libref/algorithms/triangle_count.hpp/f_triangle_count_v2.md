---
layout: function
title: triangle_count_v2
owner: Andrew Lumsdaine
brief: Two-dimensional triangle counting
tags:
  - function
defined_in_file: algorithms/triangle_count.hpp
overloads:
  "template <typename GraphT>\nsize_t triangle_count_v2(GraphT &)":
    arguments:
      - description: Adjacency graph
        name: A
        type: GraphT &
    description: Range based triangle counting similar to v0, using range based for loops and random access into the outer range.
    return: The number of triangles in the graph
    signature_with_names: "template <typename GraphT>\nsize_t triangle_count_v2(GraphT & A)"
namespace:
  - nw
  - graph
---
