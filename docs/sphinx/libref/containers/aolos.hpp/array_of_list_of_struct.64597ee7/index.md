---
layout: class
title: array_of_list_of_structs<Attributes>
owner: Andrew Lumsdaine
brief: container class for std::vector< std::forward_list< std::tuple< Attributes...>>>
tags:
  - class
defined_in_file: aolos.hpp
declaration: "template <typename... Attributes>\nclass nw::graph::array_of_list_of_structs;"
dtor: unspecified
typedefs:
  inner_container_ref:
    definition: typename std::forward_list<std::tuple<Attributes...>> &
    description: __MISSING__
  inner_iterator:
    definition: typename std::forward_list<std::tuple<Attributes...>>::iterator
    description: __MISSING__
  outer_iterator:
    definition: typename std::vector<std::forward_list<std::tuple<Attributes...>>>::iterator
    description: __MISSING__
fields:
  storage_:
    annotation:
      - private
    description: __MISSING__
    type: std::vector<std::forward_list<std::tuple<Attributes...>>>
namespace:
  - nw
  - graph
---

```{index}  array_of_list_of_structs<Attributes>
```

