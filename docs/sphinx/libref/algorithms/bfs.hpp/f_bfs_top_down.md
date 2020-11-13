---
layout: function
title: bfs_top_down
owner: Andrew Lumsdaine
brief: Top-down breadth first search
tags:
  - function
defined_in_file: algorithms/bfs.hpp
overloads:
  "template <class Graph>\nauto bfs_top_down(Graph &&, nw::graph::vertex_id_t)":
    arguments:
      - description: Adjacency graph
        name: graph
        type: Graph &&
      - description: Root of BFS search
        name: root
        type: nw::graph::vertex_id_t
    description: Top-down breadth first search uses parallel std::for_each to process frontier bins and frontier, uses nw::graph::parallel_for to process neighbor lists.
    return: Vector of parent vertices forming BFS tree
    signature_with_names: "template <class Graph>\nauto bfs_top_down(Graph && graph, nw::graph::vertex_id_t root)"
namespace:
  - nw
  - graph
---
