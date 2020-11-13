---
layout: function
title: bfs_v0
owner: Andrew Lumsdaine
brief: Breadth first search
tags:
  - function
defined_in_file: algorithms/bfs.hpp
overloads:
  "template <typename Graph>\nauto bfs_v0(Graph &, nw::graph::vertex_id_t)":
    arguments:
      - description: Adjacency graph
        name: graph
        type: Graph &
      - description: Root of BFS search
        name: root
        type: nw::graph::vertex_id_t
    description: Sequential breadth first search using std::for_each for processing frontier and neighbor lists.
    return: Vector of parent vertices forming BFS tree
    signature_with_names: "template <typename Graph>\nauto bfs_v0(Graph & graph, nw::graph::vertex_id_t root)"
namespace:
  - nw
  - graph
---
