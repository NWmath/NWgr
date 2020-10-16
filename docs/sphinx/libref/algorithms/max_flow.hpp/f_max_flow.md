---
layout: function
title: max_flow
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: algorithms/max_flow.hpp
overloads:
  "template <typename Dict, typename flowtype, typename Graph>\nflowtype max_flow(Graph &, nw::graph::vertex_id_t, nw::graph::vertex_id_t, size_t)":
    arguments:
      - description: __OPTIONAL__
        name: A
        type: Graph &
      - description: __OPTIONAL__
        name: source
        type: nw::graph::vertex_id_t
      - description: __OPTIONAL__
        name: sink
        type: nw::graph::vertex_id_t
      - description: __OPTIONAL__
        name: max_iters
        type: size_t
    description: __OPTIONAL__
    return: __OPTIONAL__
    signature_with_names: "template <typename Dict, typename flowtype, typename Graph>\nflowtype max_flow(Graph & A, nw::graph::vertex_id_t source, nw::graph::vertex_id_t sink, size_t max_iters)"
namespace:
  - nw
  - graph
---
