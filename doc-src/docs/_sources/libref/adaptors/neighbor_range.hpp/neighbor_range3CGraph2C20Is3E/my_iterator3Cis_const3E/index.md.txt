---
layout: class
title: my_iterator<is_const>
owner: __MISSING__
brief: __MISSING__
tags:
  - class
defined_in_file: adaptors/neighbor_range.hpp
declaration: "template <bool is_const>\nclass nw::graph::neighbor_range::my_iterator;"
dtor: unspecified
typedefs:
  difference_type:
    definition: std::size_t
    description: __MISSING__
  iterator_category:
    definition: std::forward_iterator_tag
    description: __MISSING__
  pointer:
    definition: nw::graph::neighbor_range::my_iterator::value_type *
    description: __MISSING__
  reference:
    definition: nw::graph::neighbor_range::my_iterator::value_type &
    description: __MISSING__
  value_type:
    definition: std::tuple<vertex_id_type>
    description: __MISSING__
fields:
  base_:
    annotation:
      - private
    description: __MISSING__
    type: nw::graph::neighbor_range::graph_iterator
  first_:
    annotation:
      - private
    description: __MISSING__
    type: nw::graph::neighbor_range::graph_iterator
  last_:
    annotation:
      - private
    description: __MISSING__
    type: nw::graph::neighbor_range::graph_iterator
namespace:
  - nw
  - graph
---
