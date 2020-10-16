---
layout: function
title: <deduction guide for splittable_range_adapter>
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: splittable_range_adapter.hpp
overloads:
  "template <class Iterator, class Range>\nauto <deduction guide for splittable_range_adapter>(Range &&) -> splittable_range_adapter<Iterator>":
    arguments:
      - description: __OPTIONAL__
        name: range
        type: Range &&
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <class Iterator, class Range>\nauto <deduction guide for splittable_range_adapter>(Range && range) -> splittable_range_adapter<Iterator>"
  "template <class Iterator, class Range>\nauto <deduction guide for splittable_range_adapter>(Range &&, std::unsigned long) -> splittable_range_adapter<Iterator>":
    arguments:
      - description: __OPTIONAL__
        name: range
        type: Range &&
      - description: __OPTIONAL__
        name: cutoff
        type: std::unsigned long
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <class Iterator, class Range>\nauto <deduction guide for splittable_range_adapter>(Range && range, std::unsigned long cutoff) -> splittable_range_adapter<Iterator>"
  "template <class Iterator>\nauto <deduction guide for splittable_range_adapter>(Iterator, Iterator) -> splittable_range_adapter<Iterator>":
    arguments:
      - description: __OPTIONAL__
        name: begin
        type: Iterator
      - description: __OPTIONAL__
        name: end
        type: Iterator
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <class Iterator>\nauto <deduction guide for splittable_range_adapter>(Iterator begin, Iterator end) -> splittable_range_adapter<Iterator>"
  "template <class Iterator>\nauto <deduction guide for splittable_range_adapter>(Iterator, Iterator, std::unsigned long) -> splittable_range_adapter<Iterator>":
    arguments:
      - description: __OPTIONAL__
        name: begin
        type: Iterator
      - description: __OPTIONAL__
        name: end
        type: Iterator
      - description: __OPTIONAL__
        name: cutoff
        type: std::unsigned long
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <class Iterator>\nauto <deduction guide for splittable_range_adapter>(Iterator begin, Iterator end, std::unsigned long cutoff) -> splittable_range_adapter<Iterator>"
  "template <class Iterator>\nauto <deduction guide for splittable_range_adapter>(const splittable_range_adapter<Iterator> &) -> splittable_range_adapter<Iterator>":
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: const splittable_range_adapter<Iterator> &
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <class Iterator>\nauto <deduction guide for splittable_range_adapter>(const splittable_range_adapter<Iterator> &) -> splittable_range_adapter<Iterator>"
  "template <class Iterator>\nauto <deduction guide for splittable_range_adapter>(splittable_range_adapter<Iterator> &&) -> splittable_range_adapter<Iterator>":
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: splittable_range_adapter<Iterator> &&
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <class Iterator>\nauto <deduction guide for splittable_range_adapter>(splittable_range_adapter<Iterator> &&) -> splittable_range_adapter<Iterator>"
  "template <class Iterator>\nauto <deduction guide for splittable_range_adapter>(splittable_range_adapter<Iterator> &) -> splittable_range_adapter<Iterator>":
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: splittable_range_adapter<Iterator> &
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <class Iterator>\nauto <deduction guide for splittable_range_adapter>(splittable_range_adapter<Iterator> &) -> splittable_range_adapter<Iterator>"
  "template <class Iterator>\nauto <deduction guide for splittable_range_adapter>(splittable_range_adapter<Iterator> &, tbb::split) -> splittable_range_adapter<Iterator>":
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
    signature_with_names: "template <class Iterator>\nauto <deduction guide for splittable_range_adapter>(splittable_range_adapter<Iterator> & rhs, tbb::split) -> splittable_range_adapter<Iterator>"
  "template <class Iterator>\nauto <deduction guide for splittable_range_adapter>(splittable_range_adapter<Iterator>) -> splittable_range_adapter<Iterator>":
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: splittable_range_adapter<Iterator>
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <class Iterator>\nauto <deduction guide for splittable_range_adapter>(splittable_range_adapter<Iterator>) -> splittable_range_adapter<Iterator>"
  "template <class Range>\nauto <deduction guide for splittable_range_adapter>(Range &&) -> splittable_range_adapter<decltype(range.begin())>":
    arguments:
      - description: __OPTIONAL__
        name: range
        type: Range &&
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <class Range>\nauto <deduction guide for splittable_range_adapter>(Range && range) -> splittable_range_adapter<decltype(range.begin())>"
  "template <class Range>\nauto <deduction guide for splittable_range_adapter>(Range &&, std::size_t) -> splittable_range_adapter<decltype(range.begin())>":
    arguments:
      - description: __OPTIONAL__
        name: range
        type: Range &&
      - description: __OPTIONAL__
        name: unnamed-1
        type: std::size_t
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <class Range>\nauto <deduction guide for splittable_range_adapter>(Range && range, std::size_t) -> splittable_range_adapter<decltype(range.begin())>"
namespace:
  - nw
  - graph
---
