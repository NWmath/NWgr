---
layout: function
title: permute_helper_all
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: build.hpp
overloads:
  "template <class edge_list_t, class adjacency_t, class Perm, size_t... Is>\nvoid permute_helper_all(edge_list_t &, adjacency_t &, std::index_sequence<Is...>, const Perm &)":
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
        name: perm
        type: const Perm &
    description: __OPTIONAL__
    return: __OPTIONAL__
    signature_with_names: "template <class edge_list_t, class adjacency_t, class Perm, size_t... Is>\nvoid permute_helper_all(edge_list_t & el, adjacency_t & cs, std::index_sequence<Is...> is, const Perm & perm)"
namespace:
  - nw
  - graph
---
