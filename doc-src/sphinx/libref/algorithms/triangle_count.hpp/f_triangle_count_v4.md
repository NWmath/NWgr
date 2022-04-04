---
layout: function
title: triangle_count_v4
owner: Andrew Lumsdaine, Luke D'Alessandro
brief: Two-dimensional triangle counting
tags:
  - function
defined_in_file: algorithms/triangle_count.hpp
overloads:
  "template <typename RandomAccessIterator>\nstd::size_t triangle_count_v4(RandomAccessIterator, RandomAccessIterator, std::size_t)":
    arguments:
      - description: The beginning of the outer range
        name: first
        type: RandomAccessIterator
      - description: The end of the outer range
        name: last
        type: RandomAccessIterator
      - description: The number of threads to use in the parallelization
        name: threads
        type: std::size_t
    description: This version of triangle counting is explicitly two-dimensional and is optimized (and only correct for) an upper-triangular graph. It uses explicit async threads to perform the parallelization.
    return: The number of triangles in the graph
    signature_with_names: "template <typename RandomAccessIterator>\nstd::size_t triangle_count_v4(RandomAccessIterator first, RandomAccessIterator last, std::size_t threads)"
namespace:
  - nw
  - graph
---
