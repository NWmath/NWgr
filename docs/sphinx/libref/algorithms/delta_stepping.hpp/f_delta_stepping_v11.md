---
layout: function
title: delta_stepping_v11
owner: Andrew Lumsdaine
brief: Delta stepping
tags:
  - function
defined_in_file: algorithms/delta_stepping.hpp
overloads:
  "template <class distance_t, class Graph, class Id, class T>\nauto delta_stepping_v11(Graph &&, Id, T)":
    arguments:
      - description: Incidence graph
        name: graph
        type: Graph &&
      - description: Source vertex
        name: source
        type: Id
      - description: Delta stepping parameter
        name: delta
        type: T
    description: Parallel frontier based delta stepping inspired by the implementation in the GAP benchmark suite. Frontier is processed with tbb::parallel_for and tbb:blocked_range. Neighbor lists are processed with tbb::parallel_for.
    return: Vector containing shortest path distances of every vertex from source
    signature_with_names: "template <class distance_t, class Graph, class Id, class T>\nauto delta_stepping_v11(Graph && graph, Id source, T delta)"
namespace:
  - nw
  - graph
---
