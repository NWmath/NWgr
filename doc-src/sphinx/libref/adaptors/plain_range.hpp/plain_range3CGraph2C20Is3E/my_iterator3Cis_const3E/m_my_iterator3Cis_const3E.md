---
layout: method
title: my_iterator<is_const>
owner: __MISSING__
brief: __MISSING__
tags:
  - method
defined_in_file: adaptors/plain_range.hpp
is_ctor: true
overloads:
  my_iterator<is_const>(const my_iterator<is_const> &):
    annotation:
      - default
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: const my_iterator<is_const> &
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: my_iterator<is_const>(const my_iterator<is_const> &)
  my_iterator<is_const>(const my_iterator<is_const> &, unsigned long):
    annotation:
      - private
    arguments:
      - description: __OPTIONAL__
        name: b
        type: const my_iterator<is_const> &
      - description: __OPTIONAL__
        name: step
        type: unsigned long
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: my_iterator<is_const>(const my_iterator<is_const> & b, unsigned long step)
  my_iterator<is_const>(nw::graph::plain_range::graph_iterator, nw::graph::plain_range::graph_iterator, nw::graph::plain_range::graph_iterator):
    arguments:
      - description: __OPTIONAL__
        name: base
        type: nw::graph::plain_range::graph_iterator
      - description: __OPTIONAL__
        name: begin
        type: nw::graph::plain_range::graph_iterator
      - description: __OPTIONAL__
        name: end
        type: nw::graph::plain_range::graph_iterator
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: my_iterator<is_const>(nw::graph::plain_range::graph_iterator base, nw::graph::plain_range::graph_iterator begin, nw::graph::plain_range::graph_iterator end)
  "template <bool was_const, class >\nmy_iterator<is_const>(const my_iterator<was_const> &)":
    arguments:
      - description: __OPTIONAL__
        name: rhs
        type: const my_iterator<was_const> &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <bool was_const, class >\nmy_iterator<is_const>(const my_iterator<was_const> & rhs)"
---
