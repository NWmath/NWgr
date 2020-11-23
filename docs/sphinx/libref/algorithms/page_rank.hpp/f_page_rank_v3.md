---
layout: function
title: page_rank_v3
owner: Andrew Lumsdaine, Scott McMillan
brief: Pagerank
tags:
  - function
defined_in_file: algorithms/page_rank.hpp
overloads:
  "template <typename Graph, typename Real>\nvoid page_rank_v3(Graph &, const std::vector<vertex_id_t> &, std::vector<Real> &, Real, Real, size_t)":
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
        type: Real
      - description: Convergence threshold
        name: threshold
        type: Real
      - description: Maximum number of iterations
        name: max_iters
        type: size_t
    description: __OPTIONAL__
    return:
    signature_with_names: "template <typename Graph, typename Real>\nvoid page_rank_v3(Graph & graph, const std::vector<vertex_id_t> & degrees, std::vector<Real> & page_rank, Real damping_factor, Real threshold, size_t max_iters)"
namespace:
  - nw
  - graph
---
