---
layout: method
title: writeMin
owner: __MISSING__
brief: __MISSING__
tags:
  - method
defined_in_file: disjoint_set.hpp
overloads:
  bool writeMin(T &, T):
    annotation:
      - private
    arguments:
      - description: __OPTIONAL__
        name: a
        type: T &
      - description: __OPTIONAL__
        name: b
        type: T
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: bool writeMin(T & a, T b)
  "template <typename F>\nbool writeMin(T &, T, F)":
    annotation:
      - private
    arguments:
      - description: __OPTIONAL__
        name: addr
        type: T &
      - description: __OPTIONAL__
        name: new_val
        type: T
      - description: __OPTIONAL__
        name: less
        type: F
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <typename F>\nbool writeMin(T & addr, T new_val, F less)"
---
