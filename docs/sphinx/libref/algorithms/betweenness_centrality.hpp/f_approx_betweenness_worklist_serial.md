---
layout: function
title: approx_betweenness_worklist_serial
owner: Andrew Lumsdaine, Kevin Deweese
brief: Weighted approximate betweenness centrality
tags:
  - function
defined_in_file: algorithms/betweenness_centrality.hpp
overloads:
  "template <typename Graph, typename score_t, typename accum_t>\nstd::vector<score_t> approx_betweenness_worklist_serial(Graph &, std::vector<vertex_id_t> &)":
    arguments:
      - description: Adjacency graph
        name: A
        type: Graph &
      - description: Vector of sources
        name: sources
        type: std::vector<vertex_id_t> &
    description: Weighted approximate betweenness centrality based on the worklist model described by Prountzos and Pingali in "Betweenness Centrality&#58; Algorithms and Implementations". Uses sequential data structures for testing.
    return: A vector containing the approximate betweenness centrality of every vertex
    signature_with_names: "template <typename Graph, typename score_t, typename accum_t>\nstd::vector<score_t> approx_betweenness_worklist_serial(Graph & A, std::vector<vertex_id_t> & sources)"
namespace:
  - nw
  - graph
---
