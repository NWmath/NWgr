---
layout: function
title: hook
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: algorithms/connected_components.hpp
overloads:
  "template <typename T>\nvoid hook(T, T, std::vector<T> &)":
    arguments:
      - description: __OPTIONAL__
        name: u
        type: T
      - description: __OPTIONAL__
        name: v
        type: T
      - description: __OPTIONAL__
        name: comp
        type: std::vector<T> &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <typename T>\nvoid hook(T u, T v, std::vector<T> & comp)"
  void hook(unsigned int, unsigned int, std::vector<unsigned int> &):
    arguments:
      - description: __OPTIONAL__
        name: u
        type: unsigned int
      - description: __OPTIONAL__
        name: v
        type: unsigned int
      - description: __OPTIONAL__
        name: comp
        type: std::vector<unsigned int> &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: void hook(unsigned int u, unsigned int v, std::vector<unsigned int> & comp)
namespace:
  - nw
  - graph
---
