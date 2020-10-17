---
layout: class
title: non_zero_iterator
owner: __MISSING__
brief: __MISSING__
tags:
  - class
defined_in_file: util/AtomicBitVector.hpp
declaration: "\nclass nw::graph::AtomicBitVector::non_zero_iterator;"
dtor: unspecified
typedefs:
  difference_type:
    definition: std::ptrdiff_t
    description: __MISSING__
  iterator_category:
    definition: std::input_iterator_tag
    description: __MISSING__
  pointer:
    definition: void
    description: __MISSING__
  reference:
    definition: const std::size_t
    description: __MISSING__
  value_type:
    definition: std::size_t
    description: __MISSING__
fields:
  bit_:
    annotation:
      - private
    description: __MISSING__
    type: std::size_t
  data_:
    annotation:
      - private
    description: __MISSING__
    type: Word *
  n_:
    annotation:
      - private
    description: __MISSING__
    type: std::size_t
  word_:
    annotation:
      - private
    description: __MISSING__
    type: std::size_t
namespace:
  - nw
  - graph
---
