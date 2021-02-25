---
layout: method
title: operator=
owner: __MISSING__
brief: __MISSING__
tags:
  - method
defined_in_file: adaptors/edge_range.hpp
overloads:
  my_iterator<is_const> & operator=(const my_iterator<false> &):
    arguments:
      - description: __OPTIONAL__
        name: rhs
        type: const my_iterator<false> &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: my_iterator<is_const> & operator=(const my_iterator<false> & rhs)
  my_iterator<is_const> & operator=(const my_iterator<is_const> &):
    annotation:
      - default
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: const my_iterator<is_const> &
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: my_iterator<is_const> & operator=(const my_iterator<is_const> &)
---
