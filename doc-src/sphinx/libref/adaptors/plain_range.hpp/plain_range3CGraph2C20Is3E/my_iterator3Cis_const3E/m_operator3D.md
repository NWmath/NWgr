---
layout: method
title: operator=
owner: __MISSING__
brief: __MISSING__
tags:
  - method
defined_in_file: adaptors/plain_range.hpp
overloads:
  my_iterator<is_const> & operator=(const my_iterator<is_const> &):
    annotation:
      - default
    arguments:
      - description: __OPTIONAL__
        name: b
        type: const my_iterator<is_const> &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: my_iterator<is_const> & operator=(const my_iterator<is_const> & b)
  "template <bool was_const, class >\nmy_iterator<is_const> & operator=(const my_iterator<was_const> &)":
    arguments:
      - description: __OPTIONAL__
        name: rhs
        type: const my_iterator<was_const> &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <bool was_const, class >\nmy_iterator<is_const> & operator=(const my_iterator<was_const> & rhs)"
---
