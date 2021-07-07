---
layout: class
title: filtered_bfs_edge_range_iterator
owner: __MISSING__
brief: __MISSING__
tags:
  - class
defined_in_file: adaptors/filtered_bfs_range.hpp
declaration: "\nclass nw::graph::filtered_bfs::filtered_bfs_edge_range::filtered_bfs_edge_range_iterator;"
dtor: unspecified
fields:
  G:
    annotation:
      - private
    description: __MISSING__
    type: typename Graph::outer_iterator
  filter_:
    annotation:
      - private
    description: __MISSING__
    type: Filter
  iterator_target_:
    annotation:
      - private
    description: __MISSING__
    type: nw::graph::filtered_bfs::filtered_bfs_edge_range::vertex_id_type
  the_range_:
    annotation:
      - private
    description: __MISSING__
    type: filtered_bfs_edge_range<Graph, Queue, Filter> &
  u_begin:
    annotation:
      - private
    description: __MISSING__
    type: typename Graph::inner_iterator
  v_:
    annotation:
      - private
    description: __MISSING__
    type: nw::graph::filtered_bfs::filtered_bfs_edge_range::vertex_id_type
namespace:
  - nw
  - graph
  - filtered_bfs
---

```{index}  filtered_bfs_edge_range_iterator
```

