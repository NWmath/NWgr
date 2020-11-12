---
layout: function
title: delta_stepping_m1
owner: Andrew Lumsdaine
brief: Delta stepping with delta=1
tags:
  - function
defined_in_file: algorithms/delta_stepping.hpp
overloads:
  "template <class distance_t, class Graph, class Id, class T>\nauto delta_stepping_m1(Graph &&, Id, T)":
    arguments:
      - description: Incidence graph
        name: graph
        type: Graph &&
      - description: __OPTIONAL__
        name: source
        type: Id
      - description: __OPTIONAL__
        name: unnamed-2
        type: T
        unnamed: true
    description: Sequential frontier based delta stepping with delta assumed to be 1.
    return: Vector containing shortest path distances of every vertex from source
    signature_with_names: "template <class distance_t, class Graph, class Id, class T>\nauto delta_stepping_m1(Graph && graph, Id source, T)"
namespace:
  - nw
  - graph
---
