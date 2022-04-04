---
layout: class
title: back_edge_range_iterator
owner: __MISSING__
brief: __MISSING__
tags:
  - class
defined_in_file: adaptors/back_edge_range.hpp
declaration: "\nclass nw::graph::back_edge_range::back_edge_range_iterator;"
dtor: unspecified
fields:
  G:
    annotation:
      - private
    description: __MISSING__
    type: typename Graph::outer_iterator
  e_begin:
    annotation:
      - private
    description: __MISSING__
    type: typename std::map<size_t, edge>::iterator
  e_end:
    annotation:
      - private
    description: __MISSING__
    type: typename std::map<size_t, edge>::iterator
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
    type: nw::graph::back_edge_range::vertex_id_type
namespace:
  - nw
  - graph
---

```{index}  back_edge_range_iterator
```

