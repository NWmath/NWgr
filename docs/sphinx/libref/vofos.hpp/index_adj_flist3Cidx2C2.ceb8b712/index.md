---
layout: class
title: index_adj_flist<idx, vertex_id, Attributes>
owner: Andrew Lumsdaine
brief: Index adjacency structure based on vector of forward list
tags:
  - class
defined_in_file: vofos.hpp
declaration: "template <int idx, class vertex_id, typename... Attributes>\nclass nw::graph::index_adj_flist;"
dtor: unspecified
typedefs:
  attributes_t:
    definition: std::tuple<Attributes...>
    description: __MISSING__
  base:
    definition: vector_of_flist_of_structs<nw::graph::index_adj_flist::vertex_id_type, Attributes...>
    description: __MISSING__
  const_inner_iterator:
    definition: typename base::const_inner_iterator
    description: __MISSING__
  const_outer_iterator:
    definition: typename base::const_outer_iterator
    description: __MISSING__
  graph_base:
    definition: nw::graph::unipartite_graph_base
    description: __MISSING__
  inner_iterator:
    definition: typename base::inner_iterator
    description: __MISSING__
  iterator:
    definition: typename base::outer_iterator
    description: __MISSING__
  num_edges_type:
    definition: typename base::size_type
    description: __MISSING__
  num_vertices_type:
    definition: std::array<typename base::size_type, 1>
    description: __MISSING__
  outer_iterator:
    definition: typename base::outer_iterator
    description: __MISSING__
  vertex_id_type:
    definition: vertex_id
    description: __MISSING__
fields:
  num_edges_:
    annotation:
      - private
    description: __MISSING__
    type: nw::graph::index_adj_flist::num_edges_type
namespace:
  - nw
  - graph
---
