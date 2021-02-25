---
layout: class
title: bfs_edge_range<Graph, Queue>
owner: __MISSING__
brief: __MISSING__
tags:
  - class
defined_in_file: adaptors/bfs_edge_range.hpp
declaration: "template <typename Graph, typename Queue>\nclass nw::graph::bfs_edge_range;"
dtor: unspecified
typedefs:
  iterator:
    definition: nw::graph::bfs_edge_range::bfs_edge_range_iterator
    description: __MISSING__
  vertex_id_type:
    annotation:
      - private
    definition: vertex_id_t<Graph>
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
  the_graph_:
    annotation:
      - private
    description: __MISSING__
    type: Graph &
namespace:
  - nw
  - graph
---

```{index}  bfs_edge_range<Graph, Queue>
```

