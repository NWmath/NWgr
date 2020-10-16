---
layout: function
title: page_rank_v1
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: algorithms/page_rank.hpp
overloads:
  "template <typename Graph, typename Real>\nvoid page_rank_v1(Graph &, const std::vector<vertex_id_t> &, std::vector<Real> &, const Real, const Real, const size_t)":
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
        type: const Real
      - description: __OPTIONAL__
        name: threshold
        type: const Real
      - description: __OPTIONAL__
        name: max_iters
        type: const size_t
    description: __OPTIONAL__
    return: __OPTIONAL__
    signature_with_names: "template <typename Graph, typename Real>\nvoid page_rank_v1(Graph & graph, const std::vector<vertex_id_t> & degrees, std::vector<Real> & page_rank, const Real damping_factor, const Real threshold, const size_t max_iters)"
namespace:
  - nw
  - graph
---
