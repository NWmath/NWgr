---
layout: class
title: edge_range_iterator
owner: __MISSING__
brief: __MISSING__
tags:
  - class
defined_in_file: adaptors/random_range.hpp
declaration: "\nclass nw::graph::random_range::edge_range_iterator;"
dtor: unspecified
typedefs:
  vertex_id_type:
    annotation:
      - private
    definition: vertex_id_t<Graph>
    description: __MISSING__
fields:
  G:
    annotation:
      - private
    description: __MISSING__
    type: typename Graph::outer_iterator
  current_vertex:
    annotation:
      - private
    description: __MISSING__
    type: nw::graph::random_range::edge_range_iterator::vertex_id_type
  length_:
    annotation:
      - private
    description: __MISSING__
    type: size_t
  the_range_:
    annotation:
      - private
    description: __MISSING__
    type: random_range<Graph> &
namespace:
  - nw
  - graph
---

```{index}  edge_range_iterator
```

