---
layout: method
title: edge_range<Attrs...>
owner: __MISSING__
brief: __MISSING__
tags:
  - method
defined_in_file: compressed.hpp
is_ctor: true
overloads:
  edge_range<Attrs...>(edge_iterator<Attrs...>, edge_iterator<Attrs...>, std::ptrdiff_t):
    arguments:
      - description: __OPTIONAL__
        name: begin
        type: edge_iterator<Attrs...>
      - description: __OPTIONAL__
        name: end
        type: edge_iterator<Attrs...>
      - description: __OPTIONAL__
        name: cutoff
        type: std::ptrdiff_t
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: edge_range<Attrs...>(edge_iterator<Attrs...> begin, edge_iterator<Attrs...> end, std::ptrdiff_t cutoff)
  edge_range<Attrs...>(edge_range<Attrs...> &, tbb::split):
    arguments:
      - description: __OPTIONAL__
        name: rhs
        type: edge_range<Attrs...> &
      - description: __OPTIONAL__
        name: unnamed-1
        type: tbb::split
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: edge_range<Attrs...>(edge_range<Attrs...> & rhs, tbb::split)
---
