---
layout: method
title: atomwrapper<T>
owner: __MISSING__
brief: __MISSING__
tags:
  - method
defined_in_file: algorithms/connected_components.hpp
is_ctor: true
overloads:
  atomwrapper<T>():
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: atomwrapper<T>()
  atomwrapper<T>(const atomwrapper<T> &):
    arguments:
      - description: __OPTIONAL__
        name: other
        type: const atomwrapper<T> &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: atomwrapper<T>(const atomwrapper<T> & other)
  atomwrapper<T>(const std::atomic<T> &):
    arguments:
      - description: __OPTIONAL__
        name: a
        type: const std::atomic<T> &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: atomwrapper<T>(const std::atomic<T> & a)
---
