---
layout: class
title: edge_list.hpp
owner: __MISSING__
brief: __MISSING__
tags:
  - class
defined_in_file: edge_list.hpp
declaration: "template <nw::graph::directedness edge_directedness, typename... Attributes>\nclass nw::graph::edge_list;"
dtor: unspecified
typedefs:
  base:
    annotation:
      - private
    definition: struct_of_arrays<nw::graph::vertex_id_t, nw::graph::vertex_id_t, Attributes...>
    description: __MISSING__
  element:
    annotation:
      - private
    definition: std::tuple<vertex_id_t, vertex_id_t, Attributes...>
    description: __MISSING__
fields:
  magic:
    description: __MISSING__
    type: const char const[16]
  max_:
    description: __MISSING__
    type: std::array<vertex_id_t, 2>
  min_:
    description: __MISSING__
    type: std::array<vertex_id_t, 2>
  prv:
    description: __MISSING__
    type: nw::graph::provenance
namespace:
  - nw
  - graph
---
