---
layout: function
title: triangle_count_edgesplit
owner: Andrew Lumsdaine, Kevin Deweese
brief: Two-dimensional triangle counting
tags:
  - function
defined_in_file: algorithms/triangle_count.hpp
overloads:
  "template <class Graph>\nstd::size_t triangle_count_edgesplit(Graph &&, std::size_t)":
    arguments:
      - description: Adjacency graph
        name: graph
        type: Graph &&
      - description: The number of threads to use in the parallelization
        name: threads
        type: std::size_t
    description: This version of triangle counting is explicitly two-dimensional. It uses explicit async threads to perform the parallelization. <br />This parallelization uses a blocked decomposition combined with a uniform edgesplit policy, where each block is assigned about the same number of edges.
    return: The number of triangles in the graph
    signature_with_names: "template <class Graph>\nstd::size_t triangle_count_edgesplit(Graph && graph, std::size_t threads)"
namespace:
  - nw
  - graph
---
