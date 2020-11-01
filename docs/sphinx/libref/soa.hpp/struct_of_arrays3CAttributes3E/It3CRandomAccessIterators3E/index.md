---
layout: class
title: It<RandomAccessIterators>
owner: __MISSING__
brief: __MISSING__
tags:
  - class
defined_in_file: soa.hpp
declaration: "template <class... RandomAccessIterators>\nclass nw::graph::struct_of_arrays::It;"
dtor: unspecified
typedefs:
  difference_type:
    definition: std::ptrdiff_t
    description: __MISSING__
  iterator_category:
    definition: std::random_access_iterator_tag
    description: __MISSING__
  pointer:
    definition: typename std::tuple<typename std::iterator_traits<RandomAccessIterators>::pointer...>
    description: __MISSING__
  reference:
    definition: typename std::tuple<typename std::iterator_traits<RandomAccessIterators>::reference...>
    description: __MISSING__
  value_type:
    definition: typename std::tuple<typename std::iterator_traits<RandomAccessIterators>::value_type...>
    description: __MISSING__
fields:
  cursor:
    annotation:
      - private
    description: __MISSING__
    type: std::size_t
  start:
    annotation:
      - private
    description: __MISSING__
    type: std::tuple<RandomAccessIterators...>
namespace:
  - nw
  - graph
---

```{index}  It<RandomAccessIterators>
```

