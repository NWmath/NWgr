---
layout: class
title: nodeinfo
owner: __MISSING__
brief: __MISSING__
tags:
  - class
defined_in_file: algorithms/betweenness_centrality.hpp
declaration: "\nstruct nodeinfo;"
ctor: unspecified
dtor: unspecified
fields:
  edge_l_s:
    description: __MISSING__
    type: std::unordered_map<vertex_id_t, std::pair<size_t, accum_t>>
  l:
    description: __MISSING__
    type: size_t
  path_counts:
    description: __MISSING__
    type: accum_t
  preds:
    description: __MISSING__
    type: std::forward_list<vertex_id_t>
  succs:
    description: __MISSING__
    type: std::forward_list<vertex_id_t>
namespace:
  - nw
  - graph
---

```{index}  nodeinfo
```

