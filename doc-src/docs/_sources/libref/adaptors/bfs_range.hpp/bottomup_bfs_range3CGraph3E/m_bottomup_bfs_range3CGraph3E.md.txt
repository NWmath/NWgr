---
layout: method
title: bottomup_bfs_range<Graph>
owner: __MISSING__
brief: __MISSING__
tags:
  - method
defined_in_file: adaptors/bfs_range.hpp
is_ctor: true
overloads:
  bottomup_bfs_range<Graph>(Graph &, nw::graph::bottomup_bfs_range::vertex_id_type):
    arguments:
      - description: __OPTIONAL__
        name: graph
        type: Graph &
      - description: __OPTIONAL__
        name: seed
        type: nw::graph::bottomup_bfs_range::vertex_id_type
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: bottomup_bfs_range<Graph>(Graph & graph, nw::graph::bottomup_bfs_range::vertex_id_type seed)
  bottomup_bfs_range<Graph>(const bottomup_bfs_range<Graph> &):
    annotation:
      - delete
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: const bottomup_bfs_range<Graph> &
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: bottomup_bfs_range<Graph>(const bottomup_bfs_range<Graph> &)
---
