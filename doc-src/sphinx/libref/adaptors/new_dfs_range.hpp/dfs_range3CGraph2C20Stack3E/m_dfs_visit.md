---
layout: method
title: dfs_visit
owner: __MISSING__
brief: __MISSING__
tags:
  - method
defined_in_file: adaptors/new_dfs_range.hpp
overloads:
  "template <typename GraphRange>\nstatic void dfs_visit(const GraphRange &, Stack &, std::vector<three_colors> &, vertex_id_t<Graph>)":
    annotation:
      - private
    arguments:
      - description: __OPTIONAL__
        name: G
        type: const GraphRange &
      - description: __OPTIONAL__
        name: Q
        type: Stack &
      - description: __OPTIONAL__
        name: colors
        type: std::vector<three_colors> &
      - description: __OPTIONAL__
        name: v
        type: vertex_id_t<Graph>
    description: __OPTIONAL__
    return: __OPTIONAL__
    signature_with_names: "template <typename GraphRange>\nstatic void dfs_visit(const GraphRange & G, Stack & Q, std::vector<three_colors> & colors, vertex_id_t<Graph> v)"
---
