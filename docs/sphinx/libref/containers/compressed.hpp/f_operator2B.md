---
layout: function
title: operator+
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: containers/compressed.hpp
overloads:
  "template <class T, typename I>\nI operator+(T, const I)":
    arguments:
      - description: __OPTIONAL__
        name: n
        type: T
      - description: __OPTIONAL__
        name: i
        type: const I
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <class T, typename I>\nI operator+(T n, const I i)"
  "template <typename index_t, typename... Attributes>\nauto operator+(typename std::iter_difference_t<typename indexed_struct_of_arrays<index_t, Attributes...>::outer_iterator>, const typename indexed_struct_of_arrays<index_t, Attributes...>::outer_iterator)":
    arguments:
      - description: __OPTIONAL__
        name: n
        type: typename std::iter_difference_t<typename indexed_struct_of_arrays<index_t, Attributes...>::outer_iterator>
      - description: __OPTIONAL__
        name: i
        type: const typename indexed_struct_of_arrays<index_t, Attributes...>::outer_iterator
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <typename index_t, typename... Attributes>\nauto operator+(typename std::iter_difference_t<typename indexed_struct_of_arrays<index_t, Attributes...>::outer_iterator> n, const typename indexed_struct_of_arrays<index_t, Attributes...>::outer_iterator i)"
namespace:
  - nw
  - graph
---
