---
layout: method
title: get_back_edge
owner: __MISSING__
brief: __MISSING__
tags:
  - method
defined_in_file: adaptors/back_edge_range.hpp
overloads:
  auto get_back_edge(nw::graph::back_edge_range::vertex_id_type, nw::graph::back_edge_range::back_edge_range_iterator &):
    arguments:
      - description: __OPTIONAL__
        name: vtx
        type: nw::graph::back_edge_range::vertex_id_type
      - description: __OPTIONAL__
        name: inner
        type: nw::graph::back_edge_range::back_edge_range_iterator &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: auto get_back_edge(nw::graph::back_edge_range::vertex_id_type vtx, nw::graph::back_edge_range::back_edge_range_iterator & inner)
  auto get_back_edge(nw::graph::back_edge_range::vertex_id_type, nw::graph::back_edge_range::vertex_id_type):
    arguments:
      - description: __OPTIONAL__
        name: vtx
        type: nw::graph::back_edge_range::vertex_id_type
      - description: __OPTIONAL__
        name: vtx2
        type: nw::graph::back_edge_range::vertex_id_type
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: auto get_back_edge(nw::graph::back_edge_range::vertex_id_type vtx, nw::graph::back_edge_range::vertex_id_type vtx2)
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
