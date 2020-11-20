---
layout: method
title: bfs_edge_range2<Graph, PriorityQueue>
owner: __MISSING__
brief: __MISSING__
tags:
  - method
defined_in_file: bfs_edge_range.hpp
is_ctor: true
overloads:
  bfs_edge_range2<Graph, PriorityQueue>(Graph &, PriorityQueue &, std::tuple<size_t, size_t>):
    arguments:
      - description: __OPTIONAL__
        name: graph
        type: Graph &
      - description: __OPTIONAL__
        name: Q
        type: PriorityQueue &
      - description: __OPTIONAL__
        name: seed
        type: std::tuple<size_t, size_t>
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: bfs_edge_range2<Graph, PriorityQueue>(Graph & graph, PriorityQueue & Q, std::tuple<size_t, size_t> seed)
  bfs_edge_range2<Graph, PriorityQueue>(const bfs_edge_range2<Graph, PriorityQueue> &&):
    annotation:
      - delete
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: const bfs_edge_range2<Graph, PriorityQueue> &&
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: bfs_edge_range2<Graph, PriorityQueue>(const bfs_edge_range2<Graph, PriorityQueue> &&)
  bfs_edge_range2<Graph, PriorityQueue>(const bfs_edge_range2<Graph, PriorityQueue> &):
    annotation:
      - delete
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: const bfs_edge_range2<Graph, PriorityQueue> &
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: bfs_edge_range2<Graph, PriorityQueue>(const bfs_edge_range2<Graph, PriorityQueue> &)
---
