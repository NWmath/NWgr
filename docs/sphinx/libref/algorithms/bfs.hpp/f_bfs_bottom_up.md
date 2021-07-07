---
layout: function
title: bfs_bottom_up
owner: Andrew Lumsdaine
brief: Bottup-up breadth first search
tags:
  - function
defined_in_file: algorithms/bfs.hpp
overloads:
  "template <class Graph, class Transpose>\nauto bfs_bottom_up(const Graph &, const Transpose &, vertex_id_t<Graph>)":
    arguments:
      - description: __OPTIONAL__
        name: g
        type: const Graph &
      - description: __OPTIONAL__
        name: gx
        type: const Transpose &
      - description: __OPTIONAL__
        name: root
        type: vertex_id_t<Graph>
    description: __OPTIONAL__
    return: __OPTIONAL__
    signature_with_names: "template <class Graph, class Transpose>\nauto bfs_bottom_up(const Graph & g, const Transpose & gx, vertex_id_t<Graph> root)"
namespace:
  - nw
  - graph
---
