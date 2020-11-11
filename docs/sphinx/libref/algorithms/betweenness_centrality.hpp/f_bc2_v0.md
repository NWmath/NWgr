---
layout: function
title: bc2_v0
owner: Andrew Lumsdaine
brief: Unweighted approximate betweenness centrality
tags:
  - function
defined_in_file: algorithms/betweenness_centrality.hpp
overloads:
  "template <typename Graph, typename score_t, typename accum_t>\nauto bc2_v0(Graph &, const std::vector<vertex_id_t>)":
    arguments:
      - description: Adjacency graph
        name: graph
        type: Graph &
      - description: Vector of sources
        name: sources
        type: const std::vector<vertex_id_t>
    description: Unweighted approximate betweenness centrality that uses a frontier based BFS to store the shortest path to every vertex from a series of source vertices. Uses this path information to approximate betweenness centrality of every vertex.
    return: A vector containing the approximate betweenness centrality of every vertex
    signature_with_names: "template <typename Graph, typename score_t, typename accum_t>\nauto bc2_v0(Graph & graph, const std::vector<vertex_id_t> sources)"
namespace:
  - nw
  - graph
---
