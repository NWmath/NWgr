---
layout: method
title: topdown_bfs_range<Graph, Queue>
owner: __MISSING__
brief: __MISSING__
tags:
  - method
defined_in_file: adaptors/bfs_range.hpp
is_ctor: true
overloads:
  topdown_bfs_range<Graph, Queue>(Graph &, nw::graph::topdown_bfs_range::vertex_id_type):
    arguments:
      - description: __OPTIONAL__
        name: graph
        type: Graph &
      - description: __OPTIONAL__
        name: seed
        type: nw::graph::topdown_bfs_range::vertex_id_type
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: topdown_bfs_range<Graph, Queue>(Graph & graph, nw::graph::topdown_bfs_range::vertex_id_type seed)
  topdown_bfs_range<Graph, Queue>(const topdown_bfs_range<Graph, Queue> &):
    annotation:
      - delete
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: const topdown_bfs_range<Graph, Queue> &
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: topdown_bfs_range<Graph, Queue>(const topdown_bfs_range<Graph, Queue> &)
---