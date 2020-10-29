---
layout: class
title: vector_of_vector_of_structs<Attributes>
owner: __MISSING__
brief: __MISSING__
tags:
  - class
defined_in_file: vovos.hpp
declaration: "template <typename... Attributes>\nclass nw::graph::vector_of_vector_of_structs;"
dtor: unspecified
typedefs:
  base:
    definition: std::vector<std::forward_list<std::tuple<Attributes...>>>
    description: __MISSING__
  inner_container_ref:
    definition: typename std::forward_list<std::tuple<Attributes...>> &
    description: __MISSING__
  inner_iterator:
    definition: typename std::forward_list<std::tuple<Attributes...>>::iterator
    description: __MISSING__
  outer_iterator:
    definition: typename std::vector<std::forward_list<std::tuple<Attributes...>>>::iterator
    description: __MISSING__
namespace:
  - nw
  - graph
---
