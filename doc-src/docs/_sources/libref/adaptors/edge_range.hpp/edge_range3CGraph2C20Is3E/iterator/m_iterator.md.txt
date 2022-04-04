---
layout: method
title: iterator
owner: __MISSING__
brief: __MISSING__
tags:
  - method
defined_in_file: edge_range.hpp
is_ctor: true
overloads:
  iterator(const nw::graph::edge_range::iterator &):
    annotation:
      - default
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: const nw::graph::edge_range::iterator &
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: iterator(const nw::graph::edge_range::iterator &)
  iterator(const nw::graph::edge_range::iterator &, unsigned long):
    annotation:
      - private
    arguments:
      - description: __OPTIONAL__
        name: b
        type: const nw::graph::edge_range::iterator &
      - description: __OPTIONAL__
        name: step
        type: unsigned long
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: iterator(const nw::graph::edge_range::iterator & b, unsigned long step)
  iterator(typename Graph::iterator, typename Graph::iterator, typename Graph::iterator):
    arguments:
      - description: __OPTIONAL__
        name: base
        type: typename Graph::iterator
      - description: __OPTIONAL__
        name: begin
        type: typename Graph::iterator
      - description: __OPTIONAL__
        name: end
        type: typename Graph::iterator
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: iterator(typename Graph::iterator base, typename Graph::iterator begin, typename Graph::iterator end)
---
