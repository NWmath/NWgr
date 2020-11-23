---
layout: method
title: dfs_edge_range<Graph, Stack>
owner: __MISSING__
brief: __MISSING__
tags:
  - method
defined_in_file: new_dfs_range.hpp
is_ctor: true
overloads:
  dfs_edge_range<Graph, Stack>(Graph &, nw::graph::vertex_id_t):
    arguments:
      - description: __OPTIONAL__
        name: graph
        type: Graph &
      - description: __OPTIONAL__
        name: seed
        type: nw::graph::vertex_id_t
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: dfs_edge_range<Graph, Stack>(Graph & graph, nw::graph::vertex_id_t seed)
  dfs_edge_range<Graph, Stack>(const dfs_edge_range<Graph, Stack> &&):
    annotation:
      - delete
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: const dfs_edge_range<Graph, Stack> &&
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: dfs_edge_range<Graph, Stack>(const dfs_edge_range<Graph, Stack> &&)
  dfs_edge_range<Graph, Stack>(const dfs_edge_range<Graph, Stack> &):
    annotation:
      - delete
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: const dfs_edge_range<Graph, Stack> &
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: dfs_edge_range<Graph, Stack>(const dfs_edge_range<Graph, Stack> &)
---
