---
layout: function
title: max_flow
owner: Andrew Lumsdaine, Kevin Deweese
brief: Edmonds-Karp max flow
tags:
  - function
defined_in_file: algorithms/max_flow.hpp
overloads:
  "template <typename Dict, typename flowtype, typename Graph>\nflowtype max_flow(Graph &, nw::graph::vertex_id_t, nw::graph::vertex_id_t, size_t)":
    arguments:
      - description: Incidence graph
        name: A
        type: Graph &
      - description: Flow source vertex
        name: source
        type: nw::graph::vertex_id_t
      - description: Flow sink vertex
        name: sink
        type: nw::graph::vertex_id_t
      - description: Maximum number of augmenting paths
        name: max_iters
        type: size_t
    description: Performs Edmonds-Karp max flow algorithm. Residual flow graph is not created explicitly. Input graph contains a flow field which is set by the function.
    return: The maximum flow amount
    signature_with_names: "template <typename Dict, typename flowtype, typename Graph>\nflowtype max_flow(Graph & A, nw::graph::vertex_id_t source, nw::graph::vertex_id_t sink, size_t max_iters)"
namespace:
  - nw
  - graph
---
