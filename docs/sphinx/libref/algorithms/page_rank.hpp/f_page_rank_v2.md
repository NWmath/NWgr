---
layout: function
title: page_rank_v2
owner: Andrew Lumsdaine, Scott McMillan
brief: Pagerank
tags:
  - function
defined_in_file: algorithms/page_rank.hpp
overloads:
  "template <typename Graph, typename Real>\nvoid page_rank_v2(Graph &, const std::vector<vertex_id_t> &, std::vector<Real> &, const Real, const Real, const size_t)":
    arguments:
      - description: Adjacency graph
        name: graph
        type: Graph &
      - description: Vector of vertex degrees
        name: degrees
        type: const std::vector<vertex_id_t> &
      - description: Vector of page rank values set by algorithm
        name: page_rank
        type: std::vector<Real> &
      - description: Damping factor, clickthrough probability
        name: damping_factor
        type: const Real
      - description: Convergence threshold
        name: threshold
        type: const Real
      - description: Maximum number of iterations
        name: max_iters
        type: const size_t
    description: __OPTIONAL__
    return: 
    signature_with_names: "template <typename Graph, typename Real>\nvoid page_rank_v2(Graph & graph, const std::vector<vertex_id_t> & degrees, std::vector<Real> & page_rank, const Real damping_factor, const Real threshold, const size_t max_iters)"
namespace:
  - nw
  - graph
---
