---
layout: function
title: betweenness_brandes
owner: Andrew Lumsdaine, Kevin Deweese
brief: Unweighted betweenness centrality
tags:
  - function
defined_in_file: algorithms/betweenness_centrality.hpp
overloads:
  "template <typename Graph, typename score_t, typename accum_t>\nstd::vector<score_t> betweenness_brandes(Graph &)":
    arguments:
      - description: Adjacency graph
        name: A
        type: Graph &
    description: Sequential Brandes algorithm for finding betweenness centrality in an unweighted graph.
    return: A vector containing the betweenness centrality of every vertex
    signature_with_names: "template <typename Graph, typename score_t, typename accum_t>\nstd::vector<score_t> betweenness_brandes(Graph & A)"
namespace:
  - nw
  - graph
---
