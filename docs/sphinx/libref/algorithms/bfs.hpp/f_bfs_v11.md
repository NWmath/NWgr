---
layout: function
title: bfs_v11
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: algorithms/bfs.hpp
overloads:
  "template <typename OutGraph, typename InGraph>\nauto bfs_v11(OutGraph &, InGraph &, nw::graph::vertex_id_t, int, int, int)":
    arguments:
      - description: __OPTIONAL__
        name: out_graph
        type: OutGraph &
      - description: __OPTIONAL__
        name: in_graph
        type: InGraph &
      - description: __OPTIONAL__
        name: root
        type: nw::graph::vertex_id_t
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
    signature_with_names: "template <typename OutGraph, typename InGraph>\nauto bfs_v11(OutGraph & out_graph, InGraph & in_graph, nw::graph::vertex_id_t root, int num_bins, int alpha, int beta)"
namespace:
  - nw
  - graph
---
