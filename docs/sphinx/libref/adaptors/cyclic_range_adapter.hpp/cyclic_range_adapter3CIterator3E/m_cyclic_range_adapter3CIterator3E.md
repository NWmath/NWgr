---
layout: method
title: cyclic_range_adapter<Iterator>
owner: __MISSING__
brief: __MISSING__
tags:
  - method
defined_in_file: adaptors/cyclic_range_adapter.hpp
is_ctor: true
overloads:
  cyclic_range_adapter<Iterator>(const cyclic_range_adapter<Iterator> &):
    annotation:
      - default
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: const cyclic_range_adapter<Iterator> &
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: cyclic_range_adapter<Iterator>(const cyclic_range_adapter<Iterator> &)
  cyclic_range_adapter<Iterator>(cyclic_range_adapter<Iterator> &&):
    annotation:
      - default
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: cyclic_range_adapter<Iterator> &&
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: cyclic_range_adapter<Iterator>(cyclic_range_adapter<Iterator> &&)
  cyclic_range_adapter<Iterator>(cyclic_range_adapter<Iterator> &, tbb::split):
    arguments:
      - description: __OPTIONAL__
        name: rhs
        type: cyclic_range_adapter<Iterator> &
      - description: __OPTIONAL__
        name: unnamed-1
        type: tbb::split
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: cyclic_range_adapter<Iterator>(cyclic_range_adapter<Iterator> & rhs, tbb::split)
  "template <class Range, class Cutoff>\ncyclic_range_adapter<Iterator>(Range &&, Cutoff)":
    arguments:
      - description: __OPTIONAL__
        name: range
        type: Range &&
      - description: __OPTIONAL__
        name: cutoff
        type: Cutoff
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <class Range, class Cutoff>\ncyclic_range_adapter<Iterator>(Range && range, Cutoff cutoff)"
---
