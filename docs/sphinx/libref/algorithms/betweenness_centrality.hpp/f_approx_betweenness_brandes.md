---
layout: function
title: approx_betweenness_brandes
owner: Andrew Lumsdaine, Kevin Deweese
brief: Approximate unweighted betweenness centrality
tags:
  - function
defined_in_file: algorithms/betweenness_centrality.hpp
overloads:
  "template <typename Graph, typename score_t, typename accum_t>\nstd::vector<score_t> approx_betweenness_brandes(Graph &, std::vector<vertex_id_t> &)":
    arguments:
      - description: Adjacency graph
        name: A
        type: Graph &
      - description: Sources vertices
        name: sources
        type: std::vector<vertex_id_t> &
    description: Sequential Brandes algorithm for finding approximate betweenness centrality in an unweighted graph.
    return: A vector containing the approximate betweenness centrality of every vertex
    signature_with_names: "template <typename Graph, typename score_t, typename accum_t>\nstd::vector<score_t> approx_betweenness_brandes(Graph & A, std::vector<vertex_id_t> & sources)"
namespace:
  - nw
  - graph
---
