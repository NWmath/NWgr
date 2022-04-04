---
layout: function
title: kruskal
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: algorithms/spanning_tree.hpp
overloads:
  "template <typename EdgeListT, typename Compare>\nEdgeListT kruskal(EdgeListT &, Compare)":
    arguments:
      - description: __OPTIONAL__
        name: E
        type: EdgeListT &
      - description: __OPTIONAL__
        name: comp
        type: Compare
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <typename EdgeListT, typename Compare>\nEdgeListT kruskal(EdgeListT & E, Compare comp)"
  "template <typename EdgeListT>\nEdgeListT kruskal(EdgeListT &)":
    arguments:
      - description: __OPTIONAL__
        name: E
        type: EdgeListT &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <typename EdgeListT>\nEdgeListT kruskal(EdgeListT & E)"
namespace:
  - nw
  - graph
---
