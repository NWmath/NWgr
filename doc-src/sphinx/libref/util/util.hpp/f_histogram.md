---
layout: function
title: histogram
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: util/util.hpp
overloads:
  "template <typename InputIterator, typename RandomAccessIterator, typename >\nvoid histogram(InputIterator, InputIterator, RandomAccessIterator, RandomAccessIterator, size_t)":
    arguments:
      - description: __OPTIONAL__
        name: first
        type: InputIterator
      - description: __OPTIONAL__
        name: last
        type: InputIterator
      - description: __OPTIONAL__
        name: o_first
        type: RandomAccessIterator
      - description: __OPTIONAL__
        name: o_last
        type: RandomAccessIterator
      - description: __OPTIONAL__
        name: idx
        type: size_t
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <typename InputIterator, typename RandomAccessIterator, typename >\nvoid histogram(InputIterator first, InputIterator last, RandomAccessIterator o_first, RandomAccessIterator o_last, size_t idx)"
  "template <typename InputIterator, typename RandomAccessIterator>\nvoid histogram(InputIterator, InputIterator, RandomAccessIterator, RandomAccessIterator)":
    arguments:
      - description: __OPTIONAL__
        name: first
        type: InputIterator
      - description: __OPTIONAL__
        name: last
        type: InputIterator
      - description: __OPTIONAL__
        name: o_first
        type: RandomAccessIterator
      - description: __OPTIONAL__
        name: o_last
        type: RandomAccessIterator
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <typename InputIterator, typename RandomAccessIterator>\nvoid histogram(InputIterator first, InputIterator last, RandomAccessIterator o_first, RandomAccessIterator o_last)"
namespace:
  - nw
  - graph
---
