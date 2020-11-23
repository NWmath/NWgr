---
layout: method
title: splittable_range_adapter<Iterator>
owner: __MISSING__
brief: __MISSING__
tags:
  - method
defined_in_file: splittable_range_adapter.hpp
is_ctor: true
overloads:
  splittable_range_adapter<Iterator>(Iterator, Iterator):
    arguments:
      - description: __OPTIONAL__
        name: begin
        type: Iterator
      - description: __OPTIONAL__
        name: end
        type: Iterator
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: splittable_range_adapter<Iterator>(Iterator begin, Iterator end)
  splittable_range_adapter<Iterator>(Iterator, Iterator, std::size_t):
    arguments:
      - description: __OPTIONAL__
        name: begin
        type: Iterator
      - description: __OPTIONAL__
        name: end
        type: Iterator
      - description: __OPTIONAL__
        name: cutoff
        type: std::size_t
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: splittable_range_adapter<Iterator>(Iterator begin, Iterator end, std::size_t cutoff)
  splittable_range_adapter<Iterator>(const splittable_range_adapter<Iterator> &):
    annotation:
      - default
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: const splittable_range_adapter<Iterator> &
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: splittable_range_adapter<Iterator>(const splittable_range_adapter<Iterator> &)
  splittable_range_adapter<Iterator>(splittable_range_adapter<Iterator> &&):
    annotation:
      - default
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: splittable_range_adapter<Iterator> &&
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: splittable_range_adapter<Iterator>(splittable_range_adapter<Iterator> &&)
  splittable_range_adapter<Iterator>(splittable_range_adapter<Iterator> &):
    annotation:
      - default
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: splittable_range_adapter<Iterator> &
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: splittable_range_adapter<Iterator>(splittable_range_adapter<Iterator> &)
  splittable_range_adapter<Iterator>(splittable_range_adapter<Iterator> &, tbb::split):
    arguments:
      - description: __OPTIONAL__
        name: rhs
        type: splittable_range_adapter<Iterator> &
      - description: __OPTIONAL__
        name: unnamed-1
        type: tbb::split
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: splittable_range_adapter<Iterator>(splittable_range_adapter<Iterator> & rhs, tbb::split)
  "template <class Range>\nexplicit splittable_range_adapter<Iterator>(Range &&)":
    arguments:
      - description: __OPTIONAL__
        name: range
        type: Range &&
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <class Range>\nexplicit splittable_range_adapter<Iterator>(Range && range)"
  "template <class Range>\nsplittable_range_adapter<Iterator>(Range &&, std::size_t)":
    arguments:
      - description: __OPTIONAL__
        name: range
        type: Range &&
      - description: __OPTIONAL__
        name: cutoff
        type: std::size_t
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <class Range>\nsplittable_range_adapter<Iterator>(Range && range, std::size_t cutoff)"
---
