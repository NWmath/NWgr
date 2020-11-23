---
layout: class
title: back_edge_range<Graph>
owner: __MISSING__
brief: __MISSING__
tags:
  - class
defined_in_file: back_edge_range.hpp
declaration: "template <typename Graph>\nclass nw::graph::back_edge_range;"
dtor: unspecified
typedefs:
  edge:
    definition: typename std::iterator_traits<typename Graph::inner_iterator>::value_type
    description: __MISSING__
  inner_iterator:
    definition: nw::graph::back_edge_range::back_edge_range_iterator
    description: __MISSING__
  outer_iterator:
    definition: nw::graph::back_edge_range::outer_back_edge_range_iterator
    description: __MISSING__
  reference:
    definition: typename std::iterator_traits<typename Graph::inner_iterator>::reference
    description: __MISSING__
fields:
  address_extra:
    annotation:
      - private
    description: __MISSING__
    type: std::vector<std::map<size_t, edge>>
  back_address:
    annotation:
      - private
    description: __MISSING__
    type: std::vector<std::map<size_t, reference>>
  back_address_extra:
    annotation:
      - private
    description: __MISSING__
    type: std::vector<std::map<size_t, edge>>
  the_graph_:
    annotation:
      - private
    description: __MISSING__
    type: Graph &
namespace:
  - nw
  - graph
---

```{index}  back_edge_range<Graph>
```

