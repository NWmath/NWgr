---
layout: function
title: perm_by_degree
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: build.hpp
overloads:
  "template <int idx, class edge_list_t, class Vector, class ExecutionPolicy>\nauto perm_by_degree(edge_list_t &, const Vector &, std::string, ExecutionPolicy &&)":
    arguments:
      - description: __OPTIONAL__
        name: el
        type: edge_list_t &
      - description: __OPTIONAL__
        name: degree
        type: const Vector &
      - description: __OPTIONAL__
        name: direction
        type: std::string
      - description: __OPTIONAL__
        name: policy
        type: ExecutionPolicy &&
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <int idx, class edge_list_t, class Vector, class ExecutionPolicy>\nauto perm_by_degree(edge_list_t & el, const Vector & degree, std::string direction, ExecutionPolicy && policy)"
  "template <int idx, class edge_list_t>\nauto perm_by_degree(edge_list_t &, std::string)":
    arguments:
      - description: __OPTIONAL__
        name: el
        type: edge_list_t &
      - description: __OPTIONAL__
        name: direction
        type: std::string
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <int idx, class edge_list_t>\nauto perm_by_degree(edge_list_t & el, std::string direction)"
namespace:
  - nw
  - graph
---
