---
layout: function
title: triangle_count_async
owner: Andrew Lumsdaine, Luke D'Alessandro
brief: Parallel triangle counting using `std::async`
tags:
  - function
defined_in_file: algorithms/triangle_count.hpp
overloads:
  "template <class Op>\nstd::size_t triangle_count_async(std::size_t, Op &&)":
    arguments:
      - description: The number of threads for `std::async`
        name: threads
        type: std::size_t
      - description: The decomposed work for each `std::async`
        name: op
        type: Op &&
    description: This version of triangle counting uses `threads` `std::async` launches to evaluate the passed `op` in parallel. The `op` will be provided the thread id, but should capture any other information required to perform the decomposed work.
    return: The += reduced total of counted triangles
    signature_with_names: "template <class Op>\nstd::size_t triangle_count_async(std::size_t threads, Op && op)"
namespace:
  - nw
  - graph
---
