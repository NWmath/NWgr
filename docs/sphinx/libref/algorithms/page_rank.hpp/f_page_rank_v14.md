---
layout: function
title: page_rank_v14
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: algorithms/page_rank.hpp
overloads:
  "template <class Graph, typename Real>\nstd::size_t page_rank_v14(Graph &&, const std::vector<vertex_id_t> &, std::vector<Real> &, Real, Real, size_t)":
    arguments:
      - description: __OPTIONAL__
        name: graph
        type: Graph &&
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
    description: __OPTIONAL__
    return: __OPTIONAL__
    signature_with_names: "template <class Graph, typename Real>\nstd::size_t page_rank_v14(Graph && graph, const std::vector<vertex_id_t> & degrees, std::vector<Real> & page_rank, Real damping_factor, Real threshold, size_t max_iters)"
namespace:
  - nw
  - graph
---
