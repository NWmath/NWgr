---
layout: function
title: triangle_count_v0
owner: Andrew Lumsdaine
brief: Naive two-dimensional triangle counting
tags:
  - function
defined_in_file: algorithms/triangle_count.hpp
overloads:
  "template <typename GraphT>\nsize_t triangle_count_v0(GraphT &)":
    arguments:
      - description: Adjacency graph
        name: A
        type: GraphT &
    description: Range based triangle counting that intersects the neighborhoods of two neighborhoods. Performs more work than necessary by intersecting the entire neighbor range of each vertex.
    return: The number of triangles in the graph
    signature_with_names: "template <typename GraphT>\nsize_t triangle_count_v0(GraphT & A)"
namespace:
  - nw
  - graph
---
