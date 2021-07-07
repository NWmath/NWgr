---
layout: function
title: relabel
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: build.hpp
overloads:
  "template <class edge_list_t, class Vector, class ExecutionPolicy>\nvoid relabel(edge_list_t &, const Vector &, ExecutionPolicy &&)":
    arguments:
      - description: __OPTIONAL__
        name: el
        type: edge_list_t &
      - description: __OPTIONAL__
        name: perm
        type: const Vector &
      - description: __OPTIONAL__
        name: policy
        type: ExecutionPolicy &&
    description: __OPTIONAL__
    return: __OPTIONAL__
    signature_with_names: "template <class edge_list_t, class Vector, class ExecutionPolicy>\nvoid relabel(edge_list_t & el, const Vector & perm, ExecutionPolicy && policy)"
namespace:
  - nw
  - graph
---
