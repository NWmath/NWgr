---
layout: method
title: perm_by_degree
owner: __MISSING__
brief: __MISSING__
tags:
  - method
defined_in_file: edge_list.hpp
overloads:
  "template <int idx, class Vector, class ExecutionPolicy>\nauto perm_by_degree(Vector &&, std::string, ExecutionPolicy &&)":
    arguments:
      - description: __OPTIONAL__
        name: degree
        type: Vector &&
      - description: __OPTIONAL__
        name: direction
        type: std::string
      - description: __OPTIONAL__
        name: policy
        type: ExecutionPolicy &&
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <int idx, class Vector, class ExecutionPolicy>\nauto perm_by_degree(Vector && degree, std::string direction, ExecutionPolicy && policy)"
  "template <int idx>\nauto perm_by_degree(std::string)":
    arguments:
      - description: __OPTIONAL__
        name: direction
        type: std::string
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <int idx>\nauto perm_by_degree(std::string direction)"
---
