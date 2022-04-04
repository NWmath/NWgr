---
layout: method
title: compare_and_swap
owner: __MISSING__
brief: __MISSING__
tags:
  - method
defined_in_file: util/disjoint_set.hpp
overloads:
  bool compare_and_swap(T &, T, T):
    annotation:
      - private
    arguments:
      - description: __OPTIONAL__
        name: x
        type: T &
      - description: __OPTIONAL__
        name: old_val
        type: T
      - description: __OPTIONAL__
        name: new_val
        type: T
    description: __OPTIONAL__
    return: __OPTIONAL__
    signature_with_names: bool compare_and_swap(T & x, T old_val, T new_val)
---
