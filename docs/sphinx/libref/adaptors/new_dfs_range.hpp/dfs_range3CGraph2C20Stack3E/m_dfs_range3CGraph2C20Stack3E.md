---
layout: method
title: dfs_range<Graph, Stack>
owner: __MISSING__
brief: __MISSING__
tags:
  - method
defined_in_file: adaptors/new_dfs_range.hpp
is_ctor: true
overloads:
  dfs_range<Graph, Stack>(Graph &, vertex_id_t<Graph>):
    arguments:
      - description: __OPTIONAL__
        name: graph
        type: Graph &
      - description: __OPTIONAL__
        name: seed
        type: vertex_id_t<Graph>
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: dfs_range<Graph, Stack>(Graph & graph, vertex_id_t<Graph> seed)
  dfs_range<Graph, Stack>(const dfs_range<Graph, Stack> &&):
    annotation:
      - delete
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: const dfs_range<Graph, Stack> &&
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: dfs_range<Graph, Stack>(const dfs_range<Graph, Stack> &&)
  dfs_range<Graph, Stack>(const dfs_range<Graph, Stack> &):
    annotation:
      - delete
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: const dfs_range<Graph, Stack> &
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: dfs_range<Graph, Stack>(const dfs_range<Graph, Stack> &)
---
