---
layout: class
title: new_dfs_range.hpp
owner: __MISSING__
brief: __MISSING__
tags:
  - class
defined_in_file: new_dfs_range.hpp
declaration: "template <typename Graph, typename Stack>\nclass nw::graph::dfs_edge_range;"
dtor: unspecified
typedefs:
  iterator:
    definition: nw::graph::dfs_edge_range::dfs_edge_range_iterator
    description: __MISSING__
fields:
  Q_:
    annotation:
      - private
    description: __MISSING__
    type: Stack
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
