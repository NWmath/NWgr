---
layout: function
title: bfs_v11
owner: Andrew Lumsdaine
brief: Breadth first search
tags:
  - function
defined_in_file: algorithms/bfs.hpp
overloads:
  "template <typename OutGraph, typename InGraph>\nauto bfs_v11(const OutGraph &, const InGraph &, vertex_id_t<OutGraph>, int, int, int)":
    arguments:
      - description: __OPTIONAL__
        name: out_graph
        type: const OutGraph &
      - description: __OPTIONAL__
        name: in_graph
        type: const InGraph &
      - description: __OPTIONAL__
        name: root
        type: vertex_id_t<OutGraph>
      - description: __OPTIONAL__
        name: num_bins
        type: int
      - description: __OPTIONAL__
        name: alpha
        type: int
      - description: __OPTIONAL__
        name: beta
        type: int
    description: __OPTIONAL__
    return: __OPTIONAL__
    signature_with_names: "template <typename OutGraph, typename InGraph>\nauto bfs_v11(const OutGraph & out_graph, const InGraph & in_graph, vertex_id_t<OutGraph> root, int num_bins, int alpha, int beta)"
namespace:
  - nw
  - graph
---
