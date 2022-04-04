---
layout: function
title: proxysort
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: util/proxysort.hpp
overloads:
  "template <typename IntT, typename Comparator, typename ThingToSort, class ExecutionPolicy>\nauto proxysort(const ThingToSort &, Comparator, ExecutionPolicy)":
    arguments:
      - description: __OPTIONAL__
        name: x
        type: const ThingToSort &
      - description: __OPTIONAL__
        name: comp
        type: Comparator
      - description: __OPTIONAL__
        name: policy
        type: ExecutionPolicy
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <typename IntT, typename Comparator, typename ThingToSort, class ExecutionPolicy>\nauto proxysort(const ThingToSort & x, Comparator comp, ExecutionPolicy policy)"
  "template <typename ThingToSort, typename Comparator, typename IntT, class ExecutionPolicy>\nvoid proxysort(const ThingToSort &, std::vector<IntT> &, Comparator, ExecutionPolicy)":
    arguments:
      - description: __OPTIONAL__
        name: x
        type: const ThingToSort &
      - description: __OPTIONAL__
        name: perm
        type: std::vector<IntT> &
      - description: __OPTIONAL__
        name: comp
        type: Comparator
      - description: __OPTIONAL__
        name: policy
        type: ExecutionPolicy
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <typename ThingToSort, typename Comparator, typename IntT, class ExecutionPolicy>\nvoid proxysort(const ThingToSort & x, std::vector<IntT> & perm, Comparator comp, ExecutionPolicy policy)"
namespace:
  - nw
  - util
---
