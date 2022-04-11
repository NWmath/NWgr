---
layout: class
title: indexed_struct_of_arrays<index_t, Attributes>
owner: __MISSING__
brief: __MISSING__
tags:
  - class
defined_in_file: containers/compressed.hpp
declaration: "template <typename index_t, typename... Attributes>\nclass nw::graph::indexed_struct_of_arrays;"
dtor: unspecified
typedefs:
  const_inner_iterator:
    definition: typename struct_of_arrays<Attributes...>::const_iterator
    description: __MISSING__
  const_iterator:
    definition: nw::graph::indexed_struct_of_arrays::const_outer_iterator
    description: __MISSING__
  const_outer_iterator:
    definition: my_outer_iterator<true>
    description: __MISSING__
  const_pointer:
    definition: typename const_iterator::pointer
    description: __MISSING__
  const_reference:
    definition: typename const_iterator::reference
    description: __MISSING__
  const_reverse_iterator:
    definition: std::reverse_iterator<const_iterator>
    description: __MISSING__
  const_sub_view:
    definition: nw::graph::splittable_range_adapter<const_inner_iterator>
    description: __MISSING__
  difference_type:
    definition: typename iterator::difference_type
    description: __MISSING__
  inner_iterator:
    definition: typename struct_of_arrays<Attributes...>::iterator
    description: __MISSING__
  iterator:
    definition: my_outer_iterator<false>
    description: __MISSING__
  outer_iterator:
    definition: my_outer_iterator<false>
    description: __MISSING__
  pointer:
    definition: typename iterator::pointer
    description: __MISSING__
  reference:
    definition: typename iterator::reference
    description: __MISSING__
  reverse_iterator:
    definition: std::reverse_iterator<iterator>
    description: __MISSING__
  size_type:
    definition: std::size_t
    description: __MISSING__
  sub_view:
    definition: nw::graph::splittable_range_adapter<inner_iterator>
    description: __MISSING__
  value_type:
    definition: typename iterator::value_type
    description: __MISSING__
fields:
  N_:
    annotation:
      - private
    description: __MISSING__
    type: index_t
  indices_:
    description: __MISSING__
    type: std::vector<index_t>
  is_open_:
    annotation:
      - private
    description: __MISSING__
    type: bool
  magic_:
    annotation:
      - private
    description: __MISSING__
    type: const char const[34]
  to_be_indexed_:
    description: __MISSING__
    type: struct_of_arrays<Attributes...>
namespace:
  - nw
  - graph
---
