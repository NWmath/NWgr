---
layout: method
title: non_zero_range
owner: __MISSING__
brief: __MISSING__
tags:
  - method
defined_in_file: util/AtomicBitVector.hpp
is_ctor: true
overloads:
  non_zero_range(Word *, std::size_t, std::size_t, std::size_t, std::size_t):
    arguments:
      - description: __OPTIONAL__
        name: data
        type: Word *
      - description: __OPTIONAL__
        name: n
        type: std::size_t
      - description: __OPTIONAL__
        name: begin
        type: std::size_t
      - description: __OPTIONAL__
        name: end
        type: std::size_t
      - description: __OPTIONAL__
        name: cutoff
        type: std::size_t
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: non_zero_range(Word * data, std::size_t n, std::size_t begin, std::size_t end, std::size_t cutoff)
  non_zero_range(const nw::graph::AtomicBitVector::non_zero_range &):
    annotation:
      - default
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: const nw::graph::AtomicBitVector::non_zero_range &
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: non_zero_range(const nw::graph::AtomicBitVector::non_zero_range &)
  non_zero_range(nw::graph::AtomicBitVector::non_zero_range &&):
    annotation:
      - default
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: nw::graph::AtomicBitVector::non_zero_range &&
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: non_zero_range(nw::graph::AtomicBitVector::non_zero_range &&)
  non_zero_range(nw::graph::AtomicBitVector::non_zero_range &, tbb::split):
    arguments:
      - description: __OPTIONAL__
        name: a
        type: nw::graph::AtomicBitVector::non_zero_range &
      - description: __OPTIONAL__
        name: unnamed-1
        type: tbb::split
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: non_zero_range(nw::graph::AtomicBitVector::non_zero_range & a, tbb::split)
---
