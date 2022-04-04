---
layout: function
title: make_adjacency
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: adjacency.hpp
overloads:
  "template <int idx, class edge_list_t, class u_integral, class ExecutionPolicy>\nauto make_adjacency(edge_list_t &, u_integral, nw::graph::directedness, ExecutionPolicy &&)":
    arguments:
      - description: __OPTIONAL__
        name: el
        type: edge_list_t &
      - description: __OPTIONAL__
        name: n
        type: u_integral
      - description: __OPTIONAL__
        name: edge_directedness
        type: nw::graph::directedness
      - description: __OPTIONAL__
        name: policy
        type: ExecutionPolicy &&
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <int idx, class edge_list_t, class u_integral, class ExecutionPolicy>\nauto make_adjacency(edge_list_t & el, u_integral n, nw::graph::directedness edge_directedness, ExecutionPolicy && policy)"
  "template <int idx, class edge_list_t>\nauto make_adjacency(edge_list_t &)":
    arguments:
      - description: __OPTIONAL__
        name: el
        type: edge_list_t &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <int idx, class edge_list_t>\nauto make_adjacency(edge_list_t & el)"
namespace:
  - nw
  - graph
---
