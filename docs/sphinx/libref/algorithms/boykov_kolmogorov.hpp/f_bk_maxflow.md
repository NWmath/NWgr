---
layout: function
title: bk_maxflow
owner: Andrew Lumsdaine, Kevin Deweese
brief: Boykov Kolmogorov Max Flow
tags:
  - function
defined_in_file: algorithms/boykov_kolmogorov.hpp
overloads:
  "template <typename Graph>\nstd::tuple<double, std::vector<tree_mem>> bk_maxflow(Graph &, std::vector<double> &)":
    arguments:
      - description: Incidence graph
        name: A
        type: Graph &
      - description: Capacity vector to source/sink
        name: cap
        type: std::vector<double> &
    description: __OPTIONAL__
    return: Tuple containing (max flow, vector of source/sink assingment)
    signature_with_names: "template <typename Graph>\nstd::tuple<double, std::vector<tree_mem>> bk_maxflow(Graph & A, std::vector<double> & cap)"
namespace:
  - nw
  - graph
---
