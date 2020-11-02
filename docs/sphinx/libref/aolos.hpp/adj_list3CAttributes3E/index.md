---
layout: class
title: adj_list<Attributes>
owner: Andrew Lumsdaine
brief: Incidence Graph implementation relying upon the array_of_list_of_structs container
tags:
  - class
defined_in_file: aolos.hpp
declaration: "template <typename... Attributes>\nclass nw::graph::adj_list;"
dtor: unspecified
typedefs:
  attributes_t:
    definition: std::tuple<Attributes...>
    description: __MISSING__
  iterator:
    definition: typename array_of_list_of_structs<size_t, Attributes...>::outer_iterator
    description: __MISSING__
namespace:
  - nw
  - graph
---

```{index}  adj_list<Attributes>
```

