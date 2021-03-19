---
layout: method
title: push_back
owner: __MISSING__
brief: __MISSING__
tags:
  - method
defined_in_file: edge_list.hpp
overloads:
  void push_back(const nw::graph::index_edge_list::element &):
    arguments:
      - description: __OPTIONAL__
        name: elem
        type: const nw::graph::index_edge_list::element &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: void push_back(const nw::graph::index_edge_list::element & elem)
  void push_back(nw::graph::index_edge_list::vertex_id_type, nw::graph::index_edge_list::vertex_id_type, Attributes...):
    arguments:
      - description: __OPTIONAL__
        name: i
        type: nw::graph::index_edge_list::vertex_id_type
      - description: __OPTIONAL__
        name: j
        type: nw::graph::index_edge_list::vertex_id_type
      - description: __OPTIONAL__
        name: attrs
        type: Attributes...
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: void push_back(nw::graph::index_edge_list::vertex_id_type i, nw::graph::index_edge_list::vertex_id_type j, Attributes... attrs)
---
