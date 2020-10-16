---
layout: method
title: find
owner: __MISSING__
brief: __MISSING__
tags:
  - method
defined_in_file: disjoint_set.hpp
overloads:
  T find(T) const:
    arguments:
      - description: __OPTIONAL__
        name: index
        type: T
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: T find(T index) const
  static T find(disjoint_set<T> &, T):
    arguments:
      - description: __OPTIONAL__
        name: s
        type: disjoint_set<T> &
      - description: __OPTIONAL__
        name: index
        type: T
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: static T find(disjoint_set<T> & s, T index)
---
