---
layout: method
title: push_back
owner: __MISSING__
brief: __MISSING__
tags:
  - method
defined_in_file: edge_list.hpp
overloads:
  void push_back(const nw::graph::edge_list::element &):
    arguments:
      - description: __OPTIONAL__
        name: elem
        type: const nw::graph::edge_list::element &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: void push_back(const nw::graph::edge_list::element & elem)
  void push_back(nw::graph::vertex_id_t, nw::graph::vertex_id_t, Attributes...):
    arguments:
      - description: __OPTIONAL__
        name: i
        type: nw::graph::vertex_id_t
      - description: __OPTIONAL__
        name: j
        type: nw::graph::vertex_id_t
      - description: __OPTIONAL__
        name: attrs
        type: Attributes...
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: void push_back(nw::graph::vertex_id_t i, nw::graph::vertex_id_t j, Attributes... attrs)
---
