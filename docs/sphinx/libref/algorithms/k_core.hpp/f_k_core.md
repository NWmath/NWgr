---
layout: function
title: k_core
owner: Andrew Lumsdaine, Kevin Deweese
brief: Find k-core
tags:
  - function
defined_in_file: algorithms/k_core.hpp
overloads:
  "template <typename Graph>\nstd::tuple<Unordered_map, size_t> k_core(const Graph &, int)":
    arguments:
      - description: __OPTIONAL__
        name: A
        type: const Graph &
      - description: __OPTIONAL__
        name: k
        type: int
    description: __OPTIONAL__
    return: __OPTIONAL__
    signature_with_names: "template <typename Graph>\nstd::tuple<Unordered_map, size_t> k_core(const Graph & A, int k)"
namespace:
  - nw
  - graph
---
