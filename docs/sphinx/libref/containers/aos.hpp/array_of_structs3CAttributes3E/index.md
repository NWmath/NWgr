---
layout: class
title: array_of_structs<Attributes>
owner: __MISSING__
brief: __MISSING__
tags:
  - class
defined_in_file: containers/aos.hpp
declaration: "template <typename... Attributes>\nclass nw::graph::array_of_structs;"
ctor: unspecified
dtor: unspecified
typedefs:
  base:
    definition: std::vector<std::tuple<Attributes...>>
    description: __MISSING__
  reference:
    definition: typename std::iterator_traits<typename storage_type::iterator>::reference
    description: __MISSING__
  storage_type:
    definition: std::vector<std::tuple<Attributes...>>
    description: __MISSING__
namespace:
  - nw
  - graph
---

```{index}  array_of_structs<Attributes>
```

