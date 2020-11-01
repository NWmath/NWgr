---
layout: class
title: bfs_edge_range2<Graph, PriorityQueue>
owner: __MISSING__
brief: __MISSING__
tags:
  - class
defined_in_file: bfs_edge_range.hpp
declaration: "template <typename Graph, typename PriorityQueue>\nclass nw::graph::bfs_edge_range2;"
dtor: unspecified
typedefs:
  iterator:
    definition: nw::graph::bfs_edge_range2::bfs_edge_range2_iterator
    description: __MISSING__
fields:
  Q_:
    annotation:
      - private
    description: __MISSING__
    type: PriorityQueue &
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

```{index}  bfs_edge_range2<Graph, PriorityQueue>
```

