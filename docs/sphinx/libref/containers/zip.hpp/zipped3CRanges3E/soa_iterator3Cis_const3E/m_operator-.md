---
layout: method
title: operator-
owner: __MISSING__
brief: __MISSING__
tags:
  - method
defined_in_file: containers/zip.hpp
overloads:
  soa_iterator<is_const> operator-(std::ptrdiff_t) const:
    arguments:
      - description: __OPTIONAL__
        name: n
        type: std::ptrdiff_t
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: soa_iterator<is_const> operator-(std::ptrdiff_t n) const
  std::ptrdiff_t operator-(const soa_iterator<is_const> &) const:
    arguments:
      - description: __OPTIONAL__
        name: b
        type: const soa_iterator<is_const> &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: std::ptrdiff_t operator-(const soa_iterator<is_const> & b) const
---
