---
layout: class
title: bfs_edge_range2_iterator
owner: __MISSING__
brief: __MISSING__
tags:
  - class
defined_in_file: adaptors/bfs_edge_range.hpp
declaration: "\nclass nw::graph::bfs_edge_range2::bfs_edge_range2_iterator;"
dtor: unspecified
fields:
  G:
    annotation:
      - private
    description: __MISSING__
    type: typename Graph::outer_iterator
  the_range_:
    annotation:
      - private
    description: __MISSING__
    type: bfs_edge_range2<Graph, PriorityQueue> &
  u_begin:
    annotation:
      - private
    description: __MISSING__
    type: typename Graph::inner_iterator
  u_end:
    annotation:
      - private
    description: __MISSING__
    type: typename Graph::inner_iterator
  v_:
    annotation:
      - private
    description: __MISSING__
    type: nw::graph::bfs_edge_range2::vertex_id_type
namespace:
  - nw
  - graph
---

```{index}  bfs_edge_range2_iterator
```
