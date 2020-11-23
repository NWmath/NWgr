---
layout: method
title: iterator
owner: __MISSING__
brief: __MISSING__
tags:
  - method
defined_in_file: plain_range.hpp
is_ctor: true
overloads:
  iterator(const nw::graph::plain_degree_range::iterator &):
    annotation:
      - default
    arguments:
      - description: __OPTIONAL__
        name: b
        type: const nw::graph::plain_degree_range::iterator &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: iterator(const nw::graph::plain_degree_range::iterator & b)
  iterator(typename Graph::iterator, typename Graph::iterator, typename Graph::iterator):
    arguments:
      - description: __OPTIONAL__
        name: base
        type: typename Graph::iterator
      - description: __OPTIONAL__
        name: first
        type: typename Graph::iterator
      - description: __OPTIONAL__
        name: last
        type: typename Graph::iterator
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: iterator(typename Graph::iterator base, typename Graph::iterator first, typename Graph::iterator last)
---
