---
layout: function
title: triangle_count_v3
owner: Andrew Lumsdaine, Luke D'Alessandro
brief: One-dimensional triangle counting
tags:
  - function
defined_in_file: algorithms/triangle_count.hpp
overloads:
  "template <class Graph>\nsize_t triangle_count_v3(Graph &&)":
    arguments:
      - description: Adjacency graph
        name: A
        type: Graph &&
    description: Edge range based triangle counting that uses a range-based for loop, compound intializer, and random access to the outer range.
    return: The number of triangles in the graph
    signature_with_names: "template <class Graph>\nsize_t triangle_count_v3(Graph && A)"
namespace:
  - nw
  - graph
---
