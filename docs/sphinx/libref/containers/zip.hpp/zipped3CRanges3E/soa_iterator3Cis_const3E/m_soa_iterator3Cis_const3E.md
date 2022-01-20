---
layout: method
title: soa_iterator<is_const>
owner: __MISSING__
brief: __MISSING__
tags:
  - method
defined_in_file: containers/zip.hpp
is_ctor: true
overloads:
  soa_iterator<is_const>():
    annotation:
      - default
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: soa_iterator<is_const>()
  soa_iterator<is_const>(const soa_iterator<false> &):
    arguments:
      - description: __OPTIONAL__
        name: b
        type: const soa_iterator<false> &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: soa_iterator<is_const>(const soa_iterator<false> & b)
  soa_iterator<is_const>(const soa_iterator<is_const> &):
    annotation:
      - default
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: const soa_iterator<is_const> &
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: soa_iterator<is_const>(const soa_iterator<is_const> &)
  soa_iterator<is_const>(nw::graph::zipped::soa_iterator::soa_t *, std::size_t):
    arguments:
      - description: __OPTIONAL__
        name: soa
        type: nw::graph::zipped::soa_iterator::soa_t *
      - description: __OPTIONAL__
        name: i
        type: std::size_t
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: soa_iterator<is_const>(nw::graph::zipped::soa_iterator::soa_t * soa, std::size_t i)
  soa_iterator<is_const>(soa_iterator<is_const> &&):
    annotation:
      - default
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: soa_iterator<is_const> &&
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: soa_iterator<is_const>(soa_iterator<is_const> &&)
---
