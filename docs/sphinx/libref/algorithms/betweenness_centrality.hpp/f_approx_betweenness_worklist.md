---
layout: function
title: approx_betweenness_worklist
owner: Andrew Lumsdaine, Kevin Deweese
brief: Weighted approximate betweenness centrality
tags:
  - function
defined_in_file: algorithms/betweenness_centrality.hpp
overloads:
  "template <typename Graph, typename score_t, typename accum_t>\nstd::vector<score_t> approx_betweenness_worklist(Graph &, std::vector<vertex_id_t> &, size_t, size_t)":
    arguments:
      - description: Adjacency graph
        name: A
        type: Graph &
      - description: Vector of sources
        name: sources
        type: std::vector<vertex_id_t> &
      - description: Number of threads to use in parallelism
        name: num_threads
        type: size_t
      - description: Delta stepping value
        name: DELTA
        type: size_t
    description: Weighted approximate betweenness centrality based on the worklist model described by Prountzos and Pingali in "Betweenness Centrality&#58; Algorithms and Implementations".
    return: A vector containing the approximate betweenness centrality of every vertex
    signature_with_names: "template <typename Graph, typename score_t, typename accum_t>\nstd::vector<score_t> approx_betweenness_worklist(Graph & A, std::vector<vertex_id_t> & sources, size_t num_threads, size_t DELTA)"
namespace:
  - nw
  - graph
---
