---
layout: function
title: bfs_bottom_up
owner: Andrew Lumsdaine
brief: Bottup-up breadth first search
tags:
  - function
defined_in_file: algorithms/bfs.hpp
overloads:
  "template <class Graph, class Transpose>\nauto bfs_bottom_up(Graph &&, Transpose &&, nw::graph::vertex_id_t)":
    arguments:
      - description: Adjacency graph (outgoing edges)
        name: g
        type: Graph &&
      - description: Adjacency graph (incoming edges)
        name: gx
        type: Transpose &&
      - description: Root of BFS search
        name: root
        type: nw::graph::vertex_id_t
    description: Bottom-up breadth first search uses tbb::parallel_reduce and tbb::blocked_range to search all the vertices for frontier parents.
    return: Vector of parent vertices forming BFS tree
    signature_with_names: "template <class Graph, class Transpose>\nauto bfs_bottom_up(Graph && g, Transpose && gx, nw::graph::vertex_id_t root)"
namespace:
  - nw
  - graph
---
