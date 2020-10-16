---
layout: function
title: <deduction guide for cyclic_range_adapter>
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: cyclic_range_adapter.hpp
overloads:
  "template <class Iterator, class Range, class Cutoff>\nauto <deduction guide for cyclic_range_adapter>(Range &&, Cutoff) -> cyclic_range_adapter<Iterator>":
    arguments:
      - description: __OPTIONAL__
        name: range
        type: Range &&
      - description: __OPTIONAL__
        name: cutoff
        type: Cutoff
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <class Iterator, class Range, class Cutoff>\nauto <deduction guide for cyclic_range_adapter>(Range && range, Cutoff cutoff) -> cyclic_range_adapter<Iterator>"
  "template <class Iterator>\nauto <deduction guide for cyclic_range_adapter>(const cyclic_range_adapter<Iterator> &) -> cyclic_range_adapter<Iterator>":
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: const cyclic_range_adapter<Iterator> &
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <class Iterator>\nauto <deduction guide for cyclic_range_adapter>(const cyclic_range_adapter<Iterator> &) -> cyclic_range_adapter<Iterator>"
  "template <class Iterator>\nauto <deduction guide for cyclic_range_adapter>(cyclic_range_adapter<Iterator> &&) -> cyclic_range_adapter<Iterator>":
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: cyclic_range_adapter<Iterator> &&
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <class Iterator>\nauto <deduction guide for cyclic_range_adapter>(cyclic_range_adapter<Iterator> &&) -> cyclic_range_adapter<Iterator>"
  "template <class Iterator>\nauto <deduction guide for cyclic_range_adapter>(cyclic_range_adapter<Iterator> &, tbb::split) -> cyclic_range_adapter<Iterator>":
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
    signature_with_names: "template <class Iterator>\nauto <deduction guide for cyclic_range_adapter>(cyclic_range_adapter<Iterator> & rhs, tbb::split) -> cyclic_range_adapter<Iterator>"
  "template <class Iterator>\nauto <deduction guide for cyclic_range_adapter>(cyclic_range_adapter<Iterator>) -> cyclic_range_adapter<Iterator>":
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: cyclic_range_adapter<Iterator>
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <class Iterator>\nauto <deduction guide for cyclic_range_adapter>(cyclic_range_adapter<Iterator>) -> cyclic_range_adapter<Iterator>"
  "template <class Range, class Cutoff>\nauto <deduction guide for cyclic_range_adapter>(Range, Cutoff) -> cyclic_range_adapter<decltype(range.begin())>":
    arguments:
      - description: __OPTIONAL__
        name: range
        type: Range
      - description: __OPTIONAL__
        name: unnamed-1
        type: Cutoff
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <class Range, class Cutoff>\nauto <deduction guide for cyclic_range_adapter>(Range range, Cutoff) -> cyclic_range_adapter<decltype(range.begin())>"
namespace:
  - nw
  - graph
---
