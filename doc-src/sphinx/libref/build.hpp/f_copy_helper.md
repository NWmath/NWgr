---
layout: function
title: copy_helper
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: build.hpp
overloads:
  "template <class edge_list_t, class adjacency_t, class ExecutionPolicy, size_t... Is>\nvoid copy_helper(edge_list_t &, adjacency_t &, std::index_sequence<Is...>, size_t, ExecutionPolicy &&)":
    arguments:
      - description: __OPTIONAL__
        name: el
        type: edge_list_t &
      - description: __OPTIONAL__
        name: cs
        type: adjacency_t &
      - description: __OPTIONAL__
        name: is
        type: std::index_sequence<Is...>
      - description: __OPTIONAL__
        name: offset
        type: size_t
      - description: __OPTIONAL__
        name: policy
        type: ExecutionPolicy &&
    description: __OPTIONAL__
    return: __OPTIONAL__
    signature_with_names: "template <class edge_list_t, class adjacency_t, class ExecutionPolicy, size_t... Is>\nvoid copy_helper(edge_list_t & el, adjacency_t & cs, std::index_sequence<Is...> is, size_t offset, ExecutionPolicy && policy)"
namespace:
  - nw
  - graph
---
