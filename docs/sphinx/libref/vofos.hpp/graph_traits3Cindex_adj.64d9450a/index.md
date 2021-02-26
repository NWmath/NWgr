---
layout: class
title: graph_traits<index_adj_flist<idx, vertex_id, Attributes...>>
owner: __MISSING__
brief: __MISSING__
tags:
  - class
defined_in_file: vofos.hpp
declaration: "\nstruct nw::graph::graph_traits;"
ctor: unspecified
dtor: unspecified
typedefs:
  const_inner_iterator:
    definition: typename inner_type::const_iterator
    description: __MISSING__
  const_outer_iterator:
    definition: typename outer_type::const_iterator
    description: __MISSING__
  inner_iterator:
    definition: typename inner_type::iterator
    description: __MISSING__
  inner_type:
    definition: std::forward_list<tuple_type>
    description: __MISSING__
  my_type:
    definition: index_adj_flist<idx, vertex_id, Attributes...>
    description: __MISSING__
  num_vertices_type:
    definition: std::array<vertex_size_type, 1>
    description: __MISSING__
  outer_iterator:
    definition: typename outer_type::iterator
    description: __MISSING__
  outer_type:
    definition: std::vector<inner_type>
    description: __MISSING__
  tuple_type:
    definition: std::tuple<Attributes...>
    description: __MISSING__
  vertex_id_type:
    definition: typename my_type::vertex_id_type
    description: __MISSING__
  vertex_size_type:
    definition: typename outer_type::size_type
    description: __MISSING__
namespace:
  - nw
  - graph
---
