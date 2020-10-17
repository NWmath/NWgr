---
layout: class
title: iterator
owner: __MISSING__
brief: __MISSING__
tags:
  - class
defined_in_file: edge_range.hpp
declaration: "\nclass nw::graph::edge_range::iterator;"
dtor: unspecified
typedefs:
  difference_type:
    definition: std::size_t
    description: __MISSING__
  iterator_category:
    definition: std::forward_iterator_tag
    description: __MISSING__
  pointer:
    definition: nw::graph::edge_range::iterator::value_type *
    description: __MISSING__
  reference:
    definition: nw::graph::edge_range::iterator::value_type &
    description: __MISSING__
  value_type:
    definition: std::tuple<vertex_id_t, vertex_id_t, std::tuple_element_t<Is, typename Graph::attributes_t>...>
    description: __MISSING__
fields:
  base_:
    annotation:
      - private
    description: __MISSING__
    type: typename Graph::iterator
  first_:
    annotation:
      - private
    description: __MISSING__
    type: typename Graph::iterator
  last_:
    annotation:
      - private
    description: __MISSING__
    type: typename Graph::iterator
  u_begin_:
    annotation:
      - private
    description: __MISSING__
    type: typename Graph::inner_iterator
  u_end_:
    annotation:
      - private
    description: __MISSING__
    type: typename Graph::inner_iterator
namespace:
  - nw
  - graph
---
