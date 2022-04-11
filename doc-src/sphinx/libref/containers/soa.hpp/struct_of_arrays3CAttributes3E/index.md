---
layout: class
title: struct_of_arrays<Attributes>
owner: __MISSING__
brief: __MISSING__
tags:
  - class
defined_in_file: containers/soa.hpp
declaration: "template <class... Attributes>\nstruct nw::graph::struct_of_arrays;"
dtor: unspecified
typedefs:
  base:
    definition: std::tuple<std::vector<Attributes>...>
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
    definition: std::tuple<std::vector<Attributes>...>
    description: __MISSING__
  value_type:
    definition: typename iterator::value_type
    description: __MISSING__
namespace:
  - nw
  - graph
---

```{index}  struct_of_arrays<Attributes>
```

