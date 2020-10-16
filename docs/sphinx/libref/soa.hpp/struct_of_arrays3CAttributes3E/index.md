---
layout: class
title: soa.hpp
owner: __MISSING__
brief: __MISSING__
tags:
  - class
defined_in_file: soa.hpp
declaration: "template <class... Attributes>\nstruct nw::graph::struct_of_arrays;"
dtor: unspecified
typedefs:
  base:
    definition: std::tuple<std::vector<Attributes>...>
    description: __MISSING__
  iterator:
    definition: It<typename std::vector<Attributes>::iterator...>
    description: __MISSING__
  storage_type:
    definition: std::tuple<std::vector<Attributes>...>
    description: __MISSING__
namespace:
  - nw
  - graph
---
