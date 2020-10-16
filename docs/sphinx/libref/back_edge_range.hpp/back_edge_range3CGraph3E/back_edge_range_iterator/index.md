---
layout: class
title: back_edge_range.hpp
owner: __MISSING__
brief: __MISSING__
tags:
  - class
defined_in_file: back_edge_range.hpp
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
    type: nw::graph::vertex_id_t
namespace:
  - nw
  - graph
---
