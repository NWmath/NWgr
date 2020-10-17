---
layout: class
title: bfs_edge_range3<Graph, Queue>
owner: __MISSING__
brief: __MISSING__
tags:
  - class
defined_in_file: bfs_edge_range.hpp
declaration: "template <typename Graph, typename Queue>\nclass nw::graph::bfs_edge_range3;"
dtor: unspecified
typedefs:
  iterator:
    definition: nw::graph::bfs_edge_range3::bfs_edge_range3_iterator
    description: __MISSING__
fields:
  Q_:
    annotation:
      - private
    description: __MISSING__
    type: Queue [2]
  colors_:
    annotation:
      - private
    description: __MISSING__
    type: std::vector<three_colors>
  the_graph_:
    annotation:
      - private
    description: __MISSING__
    type: Graph &
namespace:
  - nw
  - graph
---
