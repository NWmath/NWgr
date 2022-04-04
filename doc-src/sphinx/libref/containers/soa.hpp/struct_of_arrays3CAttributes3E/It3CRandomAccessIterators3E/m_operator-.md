---
layout: method
title: operator-
owner: __MISSING__
brief: __MISSING__
tags:
  - method
defined_in_file: soa.hpp
overloads:
  It<RandomAccessIterators...> operator-(std::size_t):
    arguments:
      - description: __OPTIONAL__
        name: offset
        type: std::size_t
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: It<RandomAccessIterators...> operator-(std::size_t offset)
  It<RandomAccessIterators...> operator-(std::size_t) const:
    arguments:
      - description: __OPTIONAL__
        name: offset
        type: std::size_t
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: It<RandomAccessIterators...> operator-(std::size_t offset) const
  nw::graph::struct_of_arrays::It::difference_type operator-(const It<RandomAccessIterators...> &) const:
    arguments:
      - description: __OPTIONAL__
        name: b
        type: const It<RandomAccessIterators...> &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: nw::graph::struct_of_arrays::It::difference_type operator-(const It<RandomAccessIterators...> & b) const
---
