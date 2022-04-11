---
layout: method
title: my_outer_iterator<is_const>
owner: __MISSING__
brief: __MISSING__
tags:
  - method
defined_in_file: containers/compressed.hpp
is_ctor: true
overloads:
  my_outer_iterator<is_const>():
    annotation:
      - default
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: my_outer_iterator<is_const>()
  my_outer_iterator<is_const>(const my_outer_iterator<false> &):
    arguments:
      - description: __OPTIONAL__
        name: rhs
        type: const my_outer_iterator<false> &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: my_outer_iterator<is_const>(const my_outer_iterator<false> & rhs)
  my_outer_iterator<is_const>(const my_outer_iterator<is_const> &):
    annotation:
      - default
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: const my_outer_iterator<is_const> &
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: my_outer_iterator<is_const>(const my_outer_iterator<is_const> &)
  my_outer_iterator<is_const>(nw::graph::indexed_struct_of_arrays::my_outer_iterator::index_it_t, nw::graph::indexed_struct_of_arrays::my_outer_iterator::indexed_it_t, index_t):
    arguments:
      - description: __OPTIONAL__
        name: indices
        type: nw::graph::indexed_struct_of_arrays::my_outer_iterator::index_it_t
      - description: __OPTIONAL__
        name: indexed
        type: nw::graph::indexed_struct_of_arrays::my_outer_iterator::indexed_it_t
      - description: __OPTIONAL__
        name: i
        type: index_t
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: my_outer_iterator<is_const>(nw::graph::indexed_struct_of_arrays::my_outer_iterator::index_it_t indices, nw::graph::indexed_struct_of_arrays::my_outer_iterator::indexed_it_t indexed, index_t i)
  my_outer_iterator<is_const>(nw::graph::indexed_struct_of_arrays::my_outer_iterator::index_iterator_t, nw::graph::indexed_struct_of_arrays::my_outer_iterator::indexed_iterator_t, index_t):
    arguments:
      - description: __OPTIONAL__
        name: indices
        type: nw::graph::indexed_struct_of_arrays::my_outer_iterator::index_iterator_t
      - description: __OPTIONAL__
        name: indexed
        type: nw::graph::indexed_struct_of_arrays::my_outer_iterator::indexed_iterator_t
      - description: __OPTIONAL__
        name: i
        type: index_t
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: my_outer_iterator<is_const>(nw::graph::indexed_struct_of_arrays::my_outer_iterator::index_iterator_t indices, nw::graph::indexed_struct_of_arrays::my_outer_iterator::indexed_iterator_t indexed, index_t i)
---
