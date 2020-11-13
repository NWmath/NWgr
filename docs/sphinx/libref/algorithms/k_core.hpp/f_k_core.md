---
layout: function
title: k_core
owner: Andrew Lumsdaine, Kevin Deweese
brief: Find k-core
tags:
  - function
defined_in_file: algorithms/k_core.hpp
overloads:
  "template <typename Graph>\nstd::tuple<Unordered_map, size_t> k_core(Graph &, int)":
    arguments:
      - description: Adjacency graph
        name: A
        type: Graph &
      - description: k-core value
        name: k
        type: int
    description: Find the k-core, the induced subgraph containing vertices with degree >= k.
    return: Tuple containing the (k_core filter, number of vertices remaining in the k-core)
    signature_with_names: "template <typename Graph>\nstd::tuple<Unordered_map, size_t> k_core(Graph & A, int k)"
namespace:
  - nw
  - graph
---
