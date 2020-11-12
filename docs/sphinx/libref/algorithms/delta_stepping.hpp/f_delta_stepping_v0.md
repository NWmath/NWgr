---
layout: function
title: delta_stepping_v0
owner: Andrew Lumsdaine
brief: Delta stepping
tags:
  - function
defined_in_file: algorithms/delta_stepping.hpp
overloads:
  "template <class distance_t, class Graph, class Id, class T>\nauto delta_stepping_v0(Graph &&, Id, T)":
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
    description: Sequential frontier based delta stepping inspired by the implementation in the GAP benchmark suite.
    return: Vector containing shortest path distances of every vertex from source
    signature_with_names: "template <class distance_t, class Graph, class Id, class T>\nauto delta_stepping_v0(Graph && graph, Id source, T delta)"
namespace:
  - nw
  - graph
---
