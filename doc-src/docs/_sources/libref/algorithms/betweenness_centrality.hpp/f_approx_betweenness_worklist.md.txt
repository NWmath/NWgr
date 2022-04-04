---
layout: function
title: approx_betweenness_worklist
owner: Andrew Lumsdaine, Kevin Deweese
brief: Weighted approximate betweenness centrality
tags:
  - function
defined_in_file: algorithms/betweenness_centrality.hpp
overloads:
  "template <typename Graph, typename score_t, typename accum_t>\nstd::vector<score_t> approx_betweenness_worklist(const Graph &, std::vector<typename Graph::vertex_id_type> &, size_t, size_t)":
    arguments:
      - description: __OPTIONAL__
        name: A
        type: const Graph &
      - description: __OPTIONAL__
        name: sources
        type: std::vector<typename Graph::vertex_id_type> &
      - description: __OPTIONAL__
        name: num_threads
        type: size_t
      - description: __OPTIONAL__
        name: DELTA
        type: size_t
    description: __OPTIONAL__
    return: __OPTIONAL__
    signature_with_names: "template <typename Graph, typename score_t, typename accum_t>\nstd::vector<score_t> approx_betweenness_worklist(const Graph & A, std::vector<typename Graph::vertex_id_type> & sources, size_t num_threads, size_t DELTA)"
namespace:
  - nw
  - graph
---
