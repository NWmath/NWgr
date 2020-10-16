---
layout: function
title: bk_maxflow2
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: algorithms/boykov_kolmogorov2.hpp
overloads:
  "template <typename Graph>\nstd::tuple<double, std::vector<bool>> bk_maxflow2(Graph &, const std::vector<double> &, std::vector<double> &)":
    arguments:
      - description: __OPTIONAL__
        name: A
        type: Graph &
      - description: __OPTIONAL__
        name: cap
        type: const std::vector<double> &
      - description: __OPTIONAL__
        name: flow
        type: std::vector<double> &
    description: __OPTIONAL__
    return: __OPTIONAL__
    signature_with_names: "template <typename Graph>\nstd::tuple<double, std::vector<bool>> bk_maxflow2(Graph & A, const std::vector<double> & cap, std::vector<double> & flow)"
namespace:
  - nw
  - graph
---
