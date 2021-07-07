---
layout: function
title: swap_to_triangular
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: build.hpp
overloads:
  "template <int idx, class edge_list_t, nw::graph::succession cessor, class ExecutionPolicy>\nvoid swap_to_triangular(edge_list_t &, ExecutionPolicy &&)":
    arguments:
      - description: __OPTIONAL__
        name: el
        type: edge_list_t &
      - description: __OPTIONAL__
        name: policy
        type: ExecutionPolicy &&
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <int idx, class edge_list_t, nw::graph::succession cessor, class ExecutionPolicy>\nvoid swap_to_triangular(edge_list_t & el, ExecutionPolicy && policy)"
  "template <int idx, class edge_list_t>\nvoid swap_to_triangular(edge_list_t &, const std::string &)":
    arguments:
      - description: __OPTIONAL__
        name: el
        type: edge_list_t &
      - description: __OPTIONAL__
        name: cessor
        type: const std::string &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <int idx, class edge_list_t>\nvoid swap_to_triangular(edge_list_t & el, const std::string & cessor)"
  "template <int idx, class edge_list_t>\nvoid swap_to_triangular(edge_list_t &, nw::graph::succession)":
    arguments:
      - description: __OPTIONAL__
        name: el
        type: edge_list_t &
      - description: __OPTIONAL__
        name: cessor
        type: nw::graph::succession
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <int idx, class edge_list_t>\nvoid swap_to_triangular(edge_list_t & el, nw::graph::succession cessor)"
namespace:
  - nw
  - graph
---
