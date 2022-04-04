---
layout: class
title: zipped<Ranges>
owner: __MISSING__
brief: __MISSING__
tags:
  - class
defined_in_file: containers/zip.hpp
declaration: "template <class... Ranges>\nstruct nw::graph::zipped;"
dtor: unspecified
typedefs:
  attributes_r:
    definition: std::tuple<typename std::iterator_traits<typename Ranges::iterator>::reference...>
    description: __MISSING__
  attributes_t:
    definition: std::tuple<typename std::iterator_traits<typename Ranges::iterator>::value_type...>
    description: __MISSING__
  base:
    definition: std::tuple<Ranges &...>
    description: __MISSING__
  const_attributes_r:
    definition: std::tuple<typename std::iterator_traits<typename Ranges::const_iterator>::reference...>
    description: __MISSING__
  const_attributes_t:
    definition: std::tuple<typename std::iterator_traits<typename Ranges::const_iterator>::value_type...>
    description: __MISSING__
  const_iterator:
    definition: soa_iterator<true>
    description: __MISSING__
  const_pointer:
    definition: typename const_iterator::pointer
    description: __MISSING__
  const_reference:
    definition: typename const_iterator::reference
    description: __MISSING__
  const_reverse_iterator:
    definition: std::reverse_iterator<const_iterator>
    description: __MISSING__
  difference_type:
    definition: typename iterator::difference_type
    description: __MISSING__
  iterator:
    definition: soa_iterator<false>
    description: __MISSING__
  pointer:
    definition: typename iterator::pointer
    description: __MISSING__
  reference:
    definition: typename iterator::reference
    description: __MISSING__
  reverse_iterator:
    definition: std::reverse_iterator<iterator>
    description: __MISSING__
  size_type:
    definition: std::size_t
    description: __MISSING__
  storage_type:
    definition: std::tuple<Ranges &...>
    description: __MISSING__
  value_type:
    definition: typename iterator::value_type
    description: __MISSING__
namespace:
  - nw
  - graph
---
