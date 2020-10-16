---
layout: function
title: page_rank_v9
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: algorithms/page_rank.hpp
overloads:
  "template <typename Graph, typename Real>\nvoid page_rank_v9(Graph &, const std::vector<vertex_id_t> &, std::vector<Real> &, Real, Real, size_t, size_t)":
    arguments:
      - description: __OPTIONAL__
        name: graph
        type: Graph &
      - description: __OPTIONAL__
        name: degrees
        type: const std::vector<vertex_id_t> &
      - description: __OPTIONAL__
        name: page_rank
        type: std::vector<Real> &
      - description: __OPTIONAL__
        name: damping_factor
        type: Real
      - description: __OPTIONAL__
        name: threshold
        type: Real
      - description: __OPTIONAL__
        name: max_iters
        type: size_t
      - description: __OPTIONAL__
        name: num_threads
        type: size_t
    description: __OPTIONAL__
    return: __OPTIONAL__
    signature_with_names: "template <typename Graph, typename Real>\nvoid page_rank_v9(Graph & graph, const std::vector<vertex_id_t> & degrees, std::vector<Real> & page_rank, Real damping_factor, Real threshold, size_t max_iters, size_t num_threads)"
namespace:
  - nw
  - graph
---
