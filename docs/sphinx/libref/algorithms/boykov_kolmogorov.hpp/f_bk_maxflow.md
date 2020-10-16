---
layout: function
title: bk_maxflow
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: algorithms/boykov_kolmogorov.hpp
overloads:
  "template <typename Graph>\nstd::tuple<double, std::vector<tree_mem>> bk_maxflow(Graph &, std::vector<double> &)":
    arguments:
      - description: __OPTIONAL__
        name: A
        type: Graph &
      - description: __OPTIONAL__
        name: cap
        type: std::vector<double> &
    description: __OPTIONAL__
    return: __OPTIONAL__
    signature_with_names: "template <typename Graph>\nstd::tuple<double, std::vector<tree_mem>> bk_maxflow(Graph & A, std::vector<double> & cap)"
namespace:
  - nw
  - graph
---
