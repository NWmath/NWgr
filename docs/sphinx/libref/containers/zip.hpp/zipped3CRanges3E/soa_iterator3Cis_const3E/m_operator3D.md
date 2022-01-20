---
layout: method
title: operator=
owner: __MISSING__
brief: __MISSING__
tags:
  - method
defined_in_file: containers/zip.hpp
overloads:
  soa_iterator<is_const> & operator=(const soa_iterator<false> &):
    arguments:
      - description: __OPTIONAL__
        name: b
        type: const soa_iterator<false> &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: soa_iterator<is_const> & operator=(const soa_iterator<false> & b)
  soa_iterator<is_const> & operator=(const soa_iterator<is_const> &):
    annotation:
      - default
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: const soa_iterator<is_const> &
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: soa_iterator<is_const> & operator=(const soa_iterator<is_const> &)
  soa_iterator<is_const> & operator=(soa_iterator<is_const> &&):
    annotation:
      - default
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: soa_iterator<is_const> &&
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: soa_iterator<is_const> & operator=(soa_iterator<is_const> &&)
---
