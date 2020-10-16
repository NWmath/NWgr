---
layout: method
title: dfs_visit
owner: __MISSING__
brief: __MISSING__
tags:
  - method
defined_in_file: new_dfs_range.hpp
overloads:
  "template <typename GraphRange>\nstatic void dfs_visit(const GraphRange &, Stack &, std::vector<three_colors> &, nw::graph::vertex_id_t)":
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
        type: nw::graph::vertex_id_t
    description: __OPTIONAL__
    return: __OPTIONAL__
    signature_with_names: "template <typename GraphRange>\nstatic void dfs_visit(const GraphRange & G, Stack & Q, std::vector<three_colors> & colors, nw::graph::vertex_id_t v)"
---
