---
layout: method
title: filtered_bfs_edge_range<Graph, Queue, Filter>
owner: __MISSING__
brief: __MISSING__
tags:
  - method
defined_in_file: adaptors/filtered_bfs_range.hpp
is_ctor: true
overloads:
  filtered_bfs_edge_range<Graph, Queue, Filter>(Graph &, nw::graph::filtered_bfs::filtered_bfs_edge_range::vertex_id_type, nw::graph::filtered_bfs::filtered_bfs_edge_range::vertex_id_type, Filter):
    arguments:
      - description: __OPTIONAL__
        name: graph
        type: Graph &
      - description: __OPTIONAL__
        name: source
        type: nw::graph::filtered_bfs::filtered_bfs_edge_range::vertex_id_type
      - description: __OPTIONAL__
        name: target
        type: nw::graph::filtered_bfs::filtered_bfs_edge_range::vertex_id_type
      - description: __OPTIONAL__
        name: filter
        type: Filter
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: filtered_bfs_edge_range<Graph, Queue, Filter>(Graph & graph, nw::graph::filtered_bfs::filtered_bfs_edge_range::vertex_id_type source, nw::graph::filtered_bfs::filtered_bfs_edge_range::vertex_id_type target, Filter filter)
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
