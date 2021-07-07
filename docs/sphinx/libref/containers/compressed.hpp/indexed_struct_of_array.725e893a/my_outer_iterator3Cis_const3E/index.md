---
layout: class
title: my_outer_iterator<is_const>
owner: __MISSING__
brief: __MISSING__
tags:
  - class
defined_in_file: containers/compressed.hpp
declaration: "template <bool is_const>\nclass nw::graph::indexed_struct_of_arrays::my_outer_iterator;"
dtor: unspecified
typedefs:
  const_index_iterator_t:
    definition: typename std::vector<index_t>::const_iterator
    description: __MISSING__
  const_indexed_iterator_t:
    definition: typename struct_of_arrays<Attributes...>::const_iterator
    description: __MISSING__
  difference_type:
    definition: std::make_signed_t<index_t>
    description: __MISSING__
  index_it_t:
    annotation:
      - private
    definition: std::conditional_t<is_const, const_index_iterator_t, index_iterator_t>
    description: __MISSING__
  index_iterator_t:
    definition: typename std::vector<index_t>::iterator
    description: __MISSING__
  indexed_it_t:
    annotation:
      - private
    definition: std::conditional_t<is_const, const_indexed_iterator_t, indexed_iterator_t>
    description: __MISSING__
  indexed_iterator_t:
    definition: typename struct_of_arrays<Attributes...>::iterator
    description: __MISSING__
  iterator_category:
    definition: std::random_access_iterator_tag
    description: __MISSING__
  pointer:
    definition: arrow_proxy<nw::graph::indexed_struct_of_arrays::my_outer_iterator::reference>
    description: __MISSING__
  reference:
    definition: nw::graph::indexed_struct_of_arrays::my_outer_iterator::value_type
    description: __MISSING__
  value_type:
    definition: std::conditional_t<is_const, const_sub_view, sub_view>
    description: __MISSING__
fields:
  i_:
    annotation:
      - private
    description: __MISSING__
    type: index_t
  indexed_:
    annotation:
      - private
    description: __MISSING__
    type: nw::graph::indexed_struct_of_arrays::my_outer_iterator::indexed_it_t
  indices_:
    annotation:
      - private
    description: __MISSING__
    type: nw::graph::indexed_struct_of_arrays::my_outer_iterator::index_it_t
namespace:
  - nw
  - graph
---
