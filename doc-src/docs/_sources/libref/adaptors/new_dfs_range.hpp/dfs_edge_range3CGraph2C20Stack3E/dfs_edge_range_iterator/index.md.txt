---
layout: class
title: dfs_edge_range_iterator
owner: __MISSING__
brief: __MISSING__
tags:
  - class
defined_in_file: adaptors/new_dfs_range.hpp
declaration: "\nclass nw::graph::dfs_edge_range::dfs_edge_range_iterator;"
dtor: unspecified
fields:
  G:
    annotation:
      - private
    description: __MISSING__
    type: typename Graph::outer_iterator
  back_edge_:
    annotation:
      - private
    description: __MISSING__
    type: bool
  has_parent_:
    annotation:
      - private
    description: __MISSING__
    type: bool
  the_range_:
    annotation:
      - private
    description: __MISSING__
    type: dfs_edge_range<Graph, Stack> &
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
  u_parent:
    annotation:
      - private
    description: __MISSING__
    type: typename Graph::inner_iterator
  v_:
    annotation:
      - private
    description: __MISSING__
    type: vertex_id_t<Graph>
namespace:
  - nw
  - graph
---

```{index}  dfs_edge_range_iterator
```

