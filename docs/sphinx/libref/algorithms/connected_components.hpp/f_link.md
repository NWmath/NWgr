---
layout: function
title: link
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: algorithms/connected_components.hpp
overloads:
  "template <typename Graph>\nvoid link(Graph &, const nw::graph::vertex_id_t, std::vector<vertex_id_t> &, const size_t)":
    arguments:
      - description: __OPTIONAL__
        name: g
        type: Graph &
      - description: __OPTIONAL__
        name: u
        type: const nw::graph::vertex_id_t
      - description: __OPTIONAL__
        name: comp
        type: std::vector<vertex_id_t> &
      - description: __OPTIONAL__
        name: neighbor_bound
        type: const size_t
    description: __OPTIONAL__
    return: __OPTIONAL__
    signature_with_names: "template <typename Graph>\nvoid link(Graph & g, const nw::graph::vertex_id_t u, std::vector<vertex_id_t> & comp, const size_t neighbor_bound)"
namespace:
  - nw
  - graph
---
