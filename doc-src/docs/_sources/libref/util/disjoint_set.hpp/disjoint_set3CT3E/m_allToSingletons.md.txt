---
layout: method
title: allToSingletons
owner: __MISSING__
brief: __MISSING__
tags:
  - method
defined_in_file: util/disjoint_set.hpp
overloads:
  "template <class ExecutionPolicy>\nvoid allToSingletons(ExecutionPolicy &&)":
    arguments:
      - description: __OPTIONAL__
        name: policy
        type: ExecutionPolicy &&
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <class ExecutionPolicy>\nvoid allToSingletons(ExecutionPolicy && policy)"
  "template <typename L>\nstatic void allToSingletons(L *, const size_t)":
    arguments:
      - description: __OPTIONAL__
        name: arr
        type: L *
      - description: __OPTIONAL__
        name: size
        type: const size_t
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <typename L>\nstatic void allToSingletons(L * arr, const size_t size)"
  "template <typename L>\nstatic void allToSingletons(std::vector<L> &, const size_t)":
    arguments:
      - description: __OPTIONAL__
        name: arr
        type: std::vector<L> &
      - description: __OPTIONAL__
        name: size
        type: const size_t
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <typename L>\nstatic void allToSingletons(std::vector<L> & arr, const size_t size)"
---
