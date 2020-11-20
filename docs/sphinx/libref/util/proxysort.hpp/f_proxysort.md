---
layout: function
title: proxysort
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: proxysort.hpp
overloads:
  "template <typename IntT, typename Comparator, typename ThingToSort>\nauto proxysort(const ThingToSort &, Comparator)":
    arguments:
      - description: __OPTIONAL__
        name: x
        type: const ThingToSort &
      - description: __OPTIONAL__
        name: comp
        type: Comparator
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <typename IntT, typename Comparator, typename ThingToSort>\nauto proxysort(const ThingToSort & x, Comparator comp)"
  "template <typename ThingToSort, typename Comparator, typename IntT>\nvoid proxysort(const ThingToSort &, std::vector<IntT> &, Comparator)":
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
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <typename ThingToSort, typename Comparator, typename IntT>\nvoid proxysort(const ThingToSort & x, std::vector<IntT> & perm, Comparator comp)"
namespace:
  - nw
  - util
---
