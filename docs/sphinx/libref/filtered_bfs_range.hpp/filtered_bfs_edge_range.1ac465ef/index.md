---
layout: class
title: filtered_bfs_range.hpp
owner: __MISSING__
brief: __MISSING__
tags:
  - class
defined_in_file: filtered_bfs_range.hpp
declaration: "template <typename Graph, typename Queue, typename Filter>\nclass nw::graph::filtered_bfs::filtered_bfs_edge_range;"
dtor: unspecified
typedefs:
  iterator:
    definition: nw::graph::filtered_bfs::filtered_bfs_edge_range::filtered_bfs_edge_range_iterator
    description: __MISSING__
fields:
  Q_:
    annotation:
      - private
    description: __MISSING__
    type: Queue
  colors_:
    annotation:
      - private
    description: __MISSING__
    type: std::vector<three_colors>
  filter_:
    annotation:
      - private
    description: __MISSING__
    type: Filter
  target_:
    annotation:
      - private
    description: __MISSING__
    type: nw::graph::vertex_id_t
  the_graph_:
    annotation:
      - private
    description: __MISSING__
    type: Graph &
  unreachable:
    description: __MISSING__
    type: bool
namespace:
  - nw
  - graph
  - filtered_bfs
---
