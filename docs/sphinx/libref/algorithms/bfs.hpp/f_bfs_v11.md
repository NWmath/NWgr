---
layout: function
title: bfs_v11
owner: Andrew Lumsdaine
brief: Breadth first search
tags:
  - function
defined_in_file: algorithms/bfs.hpp
overloads:
  "template <typename OutGraph, typename InGraph>\nauto bfs_v11(OutGraph &, InGraph &, nw::graph::vertex_id_t, int, int, int)":
    arguments:
      - description: Adjacency graph (outgoing edges)
        name: out_graph
        type: OutGraph &
      - description: Adjacency graph (incoming edges)
        name: in_graph
        type: InGraph &
      - description: Root of BFS search
        name: root
        type: nw::graph::vertex_id_t
      - description: Number of bins for storing frontier
        name: num_bins
        type: int
      - description: Direction optimized alpha parameter
        name: alpha
        type: int
      - description: Direction optizized beta parameter
        name: beta
        type: int
    description: Direction optimized breadth first search. The top-down search uses nw::graph::parallel_for and tbb::blocked_range to process the frontier bins and the frontiers, and uses nw::graph::parallel_for to process neighbor lists. The bottom-up uses tbb::parallel_reduce and tbb::blocked_range to search all the vertices for frontier parents.
    return: Vector of parent vertices forming BFS tree
    signature_with_names: "template <typename OutGraph, typename InGraph>\nauto bfs_v11(OutGraph & out_graph, InGraph & in_graph, nw::graph::vertex_id_t root, int num_bins, int alpha, int beta)"
namespace:
  - nw
  - graph
---
