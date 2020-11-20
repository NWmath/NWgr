---
layout: method
title: get_back_edge
owner: __MISSING__
brief: __MISSING__
tags:
  - method
defined_in_file: back_edge_range.hpp
overloads:
  auto get_back_edge(nw::graph::vertex_id_t, nw::graph::back_edge_range::back_edge_range_iterator &):
    arguments:
      - description: __OPTIONAL__
        name: vtx
        type: nw::graph::vertex_id_t
      - description: __OPTIONAL__
        name: inner
        type: nw::graph::back_edge_range::back_edge_range_iterator &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: auto get_back_edge(nw::graph::vertex_id_t vtx, nw::graph::back_edge_range::back_edge_range_iterator & inner)
  auto get_back_edge(nw::graph::vertex_id_t, nw::graph::vertex_id_t):
    arguments:
      - description: __OPTIONAL__
        name: vtx
        type: nw::graph::vertex_id_t
      - description: __OPTIONAL__
        name: vtx2
        type: nw::graph::vertex_id_t
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: auto get_back_edge(nw::graph::vertex_id_t vtx, nw::graph::vertex_id_t vtx2)
  auto get_back_edge(typename Graph::outer_iterator &, typename Graph::inner_iterator &):
    arguments:
      - description: __OPTIONAL__
        name: outer
        type: typename Graph::outer_iterator &
      - description: __OPTIONAL__
        name: inner
        type: typename Graph::inner_iterator &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: auto get_back_edge(typename Graph::outer_iterator & outer, typename Graph::inner_iterator & inner)
---
