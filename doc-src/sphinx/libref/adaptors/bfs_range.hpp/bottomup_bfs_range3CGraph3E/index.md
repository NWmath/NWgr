---
layout: class
title: bottomup_bfs_range<Graph>
owner: __MISSING__
brief: __MISSING__
tags:
  - class
defined_in_file: adaptors/bfs_range.hpp
declaration: "template <typename Graph>\nclass nw::graph::bottomup_bfs_range;"
dtor: unspecified
typedefs:
  Colors:
    annotation:
      - private
    definition: std::vector<status>
    description: __MISSING__
  vertex_id_type:
    annotation:
      - private
    definition: vertex_id_t<Graph>
    description: __MISSING__
fields:
  colors_:
    annotation:
      - private
    description: __MISSING__
    type: nw::graph::bottomup_bfs_range::Colors
  graph_:
    annotation:
      - private
    description: __MISSING__
    type: Graph &
  n_:
    annotation:
      - private
    description: __MISSING__
    type: size_t
  parent_v_:
    annotation:
      - private
    description: __MISSING__
    type: nw::graph::bottomup_bfs_range::vertex_id_type
  v_:
    annotation:
      - private
    description: __MISSING__
    type: nw::graph::bottomup_bfs_range::vertex_id_type
namespace:
  - nw
  - graph
---

```{index}  bottomup_bfs_range<Graph>
```
