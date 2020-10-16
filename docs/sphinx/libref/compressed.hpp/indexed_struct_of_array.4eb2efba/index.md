---
layout: class
title: compressed.hpp
owner: __MISSING__
brief: __MISSING__
tags:
  - class
defined_in_file: compressed.hpp
declaration: "template <typename... Attributes>\nclass nw::graph::indexed_struct_of_arrays;"
dtor: unspecified
typedefs:
  edge_id_t:
    definition: std::ptrdiff_t
    description: __MISSING__
  inner_iterator:
    definition: typename struct_of_arrays<Attributes...>::iterator
    description: __MISSING__
  iterator:
    definition: nw::graph::indexed_struct_of_arrays::outer_iterator
    description: __MISSING__
  sub_view:
    definition: nw::graph::splittable_range_adapter<inner_iterator>
    description: __MISSING__
fields:
  N_:
    annotation:
      - private
    description: __MISSING__
    type: nw::graph::vertex_id_t
  indices_:
    description: __MISSING__
    type: std::vector<vertex_id_t>
  is_open_:
    annotation:
      - private
    description: __MISSING__
    type: bool
  magic_:
    annotation:
      - private
    description: __MISSING__
    type: const char const[24]
  to_be_indexed_:
    description: __MISSING__
    type: struct_of_arrays<Attributes...>
namespace:
  - nw
  - graph
---
