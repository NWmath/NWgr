---
layout: function
title: relabel_by_degree
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: build.hpp
overloads:
  "template <int idx, class edge_list_t, class Vector>\nvoid relabel_by_degree(edge_list_t &, std::string, const Vector &)":
    arguments:
      - description: __OPTIONAL__
        name: el
        type: edge_list_t &
      - description: __OPTIONAL__
        name: direction
        type: std::string
      - description: __OPTIONAL__
        name: degree
        type: const Vector &
    description: __OPTIONAL__
    return: __OPTIONAL__
    signature_with_names: "template <int idx, class edge_list_t, class Vector>\nvoid relabel_by_degree(edge_list_t & el, std::string direction, const Vector & degree)"
namespace:
  - nw
  - graph
---
