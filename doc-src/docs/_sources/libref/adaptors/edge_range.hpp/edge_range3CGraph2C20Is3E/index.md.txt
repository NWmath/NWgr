---
layout: class
title: edge_range<Graph, Is>
owner: __MISSING__
brief: __MISSING__
tags:
  - class
defined_in_file: adaptors/edge_range.hpp
declaration: "template <class Graph, std::size_t... Is>\nclass nw::graph::edge_range;"
dtor: unspecified
typedefs:
  const_iterator:
    definition: my_iterator<true>
    description: __MISSING__
  graph_inner_iterator:
    annotation:
      - private
    definition: std::conditional_t<std::is_const_v<Graph>, typename Graph::const_inner_iterator, typename Graph::inner_iterator>
    description: __MISSING__
  graph_iterator:
    annotation:
      - private
    definition: std::conditional_t<std::is_const_v<Graph>, typename Graph::const_iterator, typename Graph::iterator>
    description: __MISSING__
  iterator:
    definition: my_iterator<false>
    description: __MISSING__
  vertex_id_type:
    annotation:
      - private
    definition: vertex_id_t<Graph>
    description: __MISSING__
fields:
  cutoff_:
    annotation:
      - private
    description: __MISSING__
    type: const size_t
  outer_base_:
    annotation:
      - private
    description: __MISSING__
    type: nw::graph::edge_range::graph_iterator
  outer_begin_:
    annotation:
      - private
    description: __MISSING__
    type: nw::graph::edge_range::graph_iterator
  outer_end_:
    annotation:
      - private
    description: __MISSING__
    type: nw::graph::edge_range::graph_iterator
namespace:
  - nw
  - graph
---

```{index}  edge_range<Graph, Is>
```

