---
layout: function
title: approx_betweenness_worklist
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: algorithms/betweenness_centrality.hpp
overloads:
  "template <typename Graph, typename score_t, typename accum_t>\nstd::vector<score_t> approx_betweenness_worklist(Graph &, std::vector<vertex_id_t> &, size_t, size_t)":
    arguments:
      - description: __OPTIONAL__
        name: A
        type: Graph &
      - description: __OPTIONAL__
        name: sources
        type: std::vector<vertex_id_t> &
      - description: __OPTIONAL__
        name: num_threads
        type: size_t
      - description: __OPTIONAL__
        name: DELTA
        type: size_t
    description: __OPTIONAL__
    return: __OPTIONAL__
    signature_with_names: "template <typename Graph, typename score_t, typename accum_t>\nstd::vector<score_t> approx_betweenness_worklist(Graph & A, std::vector<vertex_id_t> & sources, size_t num_threads, size_t DELTA)"
namespace:
  - nw
  - graph
---
