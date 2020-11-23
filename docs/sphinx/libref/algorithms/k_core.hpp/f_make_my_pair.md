---
layout: function
title: make_my_pair
owner: Andrew Lumsdaine, Kevin Deweese
brief: Creates neighbor pair
tags:
  - function
defined_in_file: algorithms/k_core.hpp
overloads:
  nw::graph::Neighbors make_my_pair(nw::graph::vertex_id_t, nw::graph::vertex_id_t):
    arguments:
      - description: First vertex
        name: x
        type: nw::graph::vertex_id_t
      - description: Second Vertex
        name: y
        type: nw::graph::vertex_id_t
    description: Creates neighbor pair, ensuring that the first element in the pair has a smaller vertex ID.
    return: Neighbor pair
    signature_with_names: nw::graph::Neighbors make_my_pair(nw::graph::vertex_id_t x, nw::graph::vertex_id_t y)
namespace:
  - nw
  - graph
---
