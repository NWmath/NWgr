---
layout: class
title: my_iterator<is_const>
owner: __MISSING__
brief: __MISSING__
tags:
  - class
defined_in_file: adaptors/edge_range.hpp
declaration: "template <bool is_const>\nclass nw::graph::edge_range::my_iterator;"
dtor: unspecified
typedefs:
  difference_type:
    definition: std::ptrdiff_t
    description: __MISSING__
  iterator_category:
    definition: std::forward_iterator_tag
    description: __MISSING__
  pointer:
    definition: arrow_proxy<nw::graph::edge_range::my_iterator::reference>
    description: __MISSING__
  reference:
    definition: std::tuple<vertex_id_type, vertex_id_type, typename std::tuple_element_t<Is, std::conditional_t<is_const, const typename Graph::attributes_t, typename Graph::attributes_t>> &...>
    description: __MISSING__
  value_type:
    definition: std::tuple<vertex_id_type, vertex_id_type, typename std::tuple_element_t<Is, std::conditional_t<is_const, const typename Graph::attributes_t, typename Graph::attributes_t>>...>
    description: __MISSING__
fields:
  base_:
    annotation:
      - private
    description: __MISSING__
    type: nw::graph::edge_range::graph_iterator
  first_:
    annotation:
      - private
    description: __MISSING__
    type: nw::graph::edge_range::graph_iterator
  last_:
    annotation:
      - private
    description: __MISSING__
    type: nw::graph::edge_range::graph_iterator
  u_begin_:
    annotation:
      - private
    description: __MISSING__
    type: nw::graph::edge_range::graph_inner_iterator
  u_end_:
    annotation:
      - private
    description: __MISSING__
    type: nw::graph::edge_range::graph_inner_iterator
namespace:
  - nw
  - graph
---
