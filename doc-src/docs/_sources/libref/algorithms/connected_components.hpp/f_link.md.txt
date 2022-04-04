---
layout: function
title: link
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: algorithms/connected_components.hpp
overloads:
  "template <typename Graph, typename T>\nvoid link(const Graph &, const T, std::vector<T> &, const size_t)":
    arguments:
      - description: __OPTIONAL__
        name: g
        type: const Graph &
      - description: __OPTIONAL__
        name: u
        type: const T
      - description: __OPTIONAL__
        name: comp
        type: std::vector<T> &
      - description: __OPTIONAL__
        name: neighbor_bound
        type: const size_t
    description: __OPTIONAL__
    return: __OPTIONAL__
    signature_with_names: "template <typename Graph, typename T>\nvoid link(const Graph & g, const T u, std::vector<T> & comp, const size_t neighbor_bound)"
namespace:
  - nw
  - graph
---
