---
layout: class
title: vector_of_flist_of_structs<Attributes>
owner: __MISSING__
brief: __MISSING__
tags:
  - class
defined_in_file: vofos.hpp
declaration: "template <typename... Attributes>\nclass nw::graph::vector_of_flist_of_structs;"
dtor: unspecified
typedefs:
  base:
    definition: std::vector<std::forward_list<std::tuple<Attributes...>>>
    description: __MISSING__
  const_inner_iterator:
    definition: typename inner::const_iterator
    description: __MISSING__
  const_outer_iterator:
    definition: typename base::const_iterator
    description: __MISSING__
  inner:
    definition: std::forward_list<std::tuple<Attributes...>>
    description: __MISSING__
  inner_container_ref:
    definition: nw::graph::vector_of_flist_of_structs::inner &
    description: __MISSING__
  inner_iterator:
    definition: typename inner::iterator
    description: __MISSING__
  outer_iterator:
    definition: typename base::iterator
    description: __MISSING__
namespace:
  - nw
  - graph
---
