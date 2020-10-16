---
layout: function
title: parallel_for_sequential
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: util/parallel_for.hpp
overloads:
  "template <class Range, class Op, class Reduce, class T>\nauto parallel_for_sequential(Range &&, Op &&, Reduce &&, T)":
    arguments:
      - description: __OPTIONAL__
        name: range
        type: Range &&
      - description: __OPTIONAL__
        name: op
        type: Op &&
      - description: __OPTIONAL__
        name: reduce
        type: Reduce &&
      - description: __OPTIONAL__
        name: init
        type: T
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <class Range, class Op, class Reduce, class T>\nauto parallel_for_sequential(Range && range, Op && op, Reduce && reduce, T init)"
  "template <class Range, class Op>\nvoid parallel_for_sequential(Range &&, Op &&)":
    arguments:
      - description: __OPTIONAL__
        name: range
        type: Range &&
      - description: __OPTIONAL__
        name: op
        type: Op &&
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <class Range, class Op>\nvoid parallel_for_sequential(Range && range, Op && op)"
namespace:
  - nw
  - graph
---
