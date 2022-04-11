---
layout: method
title: bfs_edge_range<Graph, Queue>
owner: __MISSING__
brief: __MISSING__
tags:
  - method
defined_in_file: adaptors/bfs_edge_range.hpp
is_ctor: true
overloads:
  bfs_edge_range<Graph, Queue>(Graph &, nw::graph::bfs_edge_range::vertex_id_type):
    arguments:
      - description: __OPTIONAL__
        name: graph
        type: Graph &
      - description: __OPTIONAL__
        name: seed
        type: nw::graph::bfs_edge_range::vertex_id_type
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: bfs_edge_range<Graph, Queue>(Graph & graph, nw::graph::bfs_edge_range::vertex_id_type seed)
  bfs_edge_range<Graph, Queue>(const bfs_edge_range<Graph, Queue> &&):
    annotation:
      - delete
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: const bfs_edge_range<Graph, Queue> &&
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: bfs_edge_range<Graph, Queue>(const bfs_edge_range<Graph, Queue> &&)
  bfs_edge_range<Graph, Queue>(const bfs_edge_range<Graph, Queue> &):
    annotation:
      - delete
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: const bfs_edge_range<Graph, Queue> &
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: bfs_edge_range<Graph, Queue>(const bfs_edge_range<Graph, Queue> &)
---
