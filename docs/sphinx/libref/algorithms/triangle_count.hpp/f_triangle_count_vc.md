---
layout: function
title: triangle_count_vc
owner: Andrew Lumsdaine, Luke D'Alessandro
brief: Triangle count verifier
tags:
  - function
defined_in_file: algorithms/triangle_count.hpp
overloads:
  "template <typename GraphT>\nsize_t triangle_count_vc(GraphT &)":
    arguments:
      - description: Adjacency graph
        name: A
        type: GraphT &
    description: Sequential triangle count verifier using pointers to the neighbor lists.
    return: The number of triangles in the graph
    signature_with_names: "template <typename GraphT>\nsize_t triangle_count_vc(GraphT & A)"
namespace:
  - nw
  - graph
---
