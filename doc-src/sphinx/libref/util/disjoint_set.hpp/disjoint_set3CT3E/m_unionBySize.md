---
layout: method
title: unionBySize
owner: __MISSING__
brief: __MISSING__
tags:
  - method
defined_in_file: util/disjoint_set.hpp
overloads:
  static void unionBySize(disjoint_set<T> &, const T &, const T &):
    arguments:
      - description: __OPTIONAL__
        name: s
        type: disjoint_set<T> &
      - description: __OPTIONAL__
        name: u
        type: const T &
      - description: __OPTIONAL__
        name: v
        type: const T &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: static void unionBySize(disjoint_set<T> & s, const T & u, const T & v)
  void unionBySize(const T &, const T &):
    arguments:
      - description: __OPTIONAL__
        name: u
        type: const T &
      - description: __OPTIONAL__
        name: v
        type: const T &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: void unionBySize(const T & u, const T & v)
---
