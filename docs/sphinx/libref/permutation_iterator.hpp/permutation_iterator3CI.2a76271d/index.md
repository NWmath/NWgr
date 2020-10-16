---
layout: class
title: permutation_iterator.hpp
owner: __MISSING__
brief: __MISSING__
tags:
  - class
defined_in_file: permutation_iterator.hpp
declaration: "template <class Iterator, class Permutation>\nclass nw::graph::permutation_iterator;"
dtor: unspecified
typedefs:
  difference_type:
    definition: typename std::iterator_traits<Iterator>::difference_type
    description: __MISSING__
  iterator_category:
    definition: std::input_iterator_tag
    description: __MISSING__
  pointer:
    definition: typename std::iterator_traits<Iterator>::pointer
    description: __MISSING__
  reference:
    definition: typename std::iterator_traits<Iterator>::reference
    description: __MISSING__
  value_type:
    definition: typename std::iterator_traits<Iterator>::value_type
    description: __MISSING__
fields:
  base_:
    annotation:
      - private
    description: __MISSING__
    type: Iterator
  i_:
    annotation:
      - private
    description: __MISSING__
    type: nw::graph::permutation_iterator::difference_type
  perm_:
    annotation:
      - private
    description: __MISSING__
    type: Permutation
namespace:
  - nw
  - graph
---
