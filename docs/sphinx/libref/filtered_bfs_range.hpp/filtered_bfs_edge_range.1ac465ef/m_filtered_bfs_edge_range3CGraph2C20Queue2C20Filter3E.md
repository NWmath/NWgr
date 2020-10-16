---
layout: method
title: filtered_bfs_edge_range<Graph, Queue, Filter>
owner: __MISSING__
brief: __MISSING__
tags:
  - method
defined_in_file: filtered_bfs_range.hpp
is_ctor: true
overloads:
  filtered_bfs_edge_range<Graph, Queue, Filter>(Graph &, nw::graph::vertex_id_t, nw::graph::vertex_id_t, Filter):
    arguments:
      - description: __OPTIONAL__
        name: graph
        type: Graph &
      - description: __OPTIONAL__
        name: source
        type: nw::graph::vertex_id_t
      - description: __OPTIONAL__
        name: target
        type: nw::graph::vertex_id_t
      - description: __OPTIONAL__
        name: filter
        type: Filter
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: filtered_bfs_edge_range<Graph, Queue, Filter>(Graph & graph, nw::graph::vertex_id_t source, nw::graph::vertex_id_t target, Filter filter)
  filtered_bfs_edge_range<Graph, Queue, Filter>(const filtered_bfs_edge_range<Graph, Queue, Filter> &&):
    annotation:
      - delete
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: const filtered_bfs_edge_range<Graph, Queue, Filter> &&
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: filtered_bfs_edge_range<Graph, Queue, Filter>(const filtered_bfs_edge_range<Graph, Queue, Filter> &&)
  filtered_bfs_edge_range<Graph, Queue, Filter>(const filtered_bfs_edge_range<Graph, Queue, Filter> &):
    annotation:
      - delete
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: const filtered_bfs_edge_range<Graph, Queue, Filter> &
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: filtered_bfs_edge_range<Graph, Queue, Filter>(const filtered_bfs_edge_range<Graph, Queue, Filter> &)
---
