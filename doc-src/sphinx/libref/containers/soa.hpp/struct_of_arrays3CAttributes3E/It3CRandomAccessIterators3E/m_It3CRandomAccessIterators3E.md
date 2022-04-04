---
layout: method
title: It<RandomAccessIterators...>
owner: __MISSING__
brief: __MISSING__
tags:
  - method
defined_in_file: soa.hpp
is_ctor: true
overloads:
  It<RandomAccessIterators...>():
    annotation:
      - default
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: It<RandomAccessIterators...>()
  It<RandomAccessIterators...>(It<RandomAccessIterators...> &&):
    annotation:
      - default
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: It<RandomAccessIterators...> &&
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: It<RandomAccessIterators...>(It<RandomAccessIterators...> &&)
  It<RandomAccessIterators...>(const It<RandomAccessIterators...> &):
    annotation:
      - default
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: const It<RandomAccessIterators...> &
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: It<RandomAccessIterators...>(const It<RandomAccessIterators...> &)
  It<RandomAccessIterators...>(const std::tuple<RandomAccessIterators...> &, std::size_t):
    annotation:
      - private
    arguments:
      - description: __OPTIONAL__
        name: iters
        type: const std::tuple<RandomAccessIterators...> &
      - description: __OPTIONAL__
        name: init
        type: std::size_t
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: It<RandomAccessIterators...>(const std::tuple<RandomAccessIterators...> & iters, std::size_t init)
  explicit It<RandomAccessIterators...>(const RandomAccessIterators...):
    arguments:
      - description: __OPTIONAL__
        name: iters
        type: const RandomAccessIterators...
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: explicit It<RandomAccessIterators...>(const RandomAccessIterators... iters)
---
