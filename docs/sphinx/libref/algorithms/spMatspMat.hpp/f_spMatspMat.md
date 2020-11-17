---
layout: function
title: spMatspMat
owner: Andrew Lumsdaine, Scott McMillan
brief: A*B
tags:
  - function
defined_in_file: algorithms/spMatspMat.hpp
overloads:
  "template <typename ScalarT, typename LGraphT, typename RGraphT, typename MapOpT, typename ReduceOpT>\nedge_list<directed, ScalarT> spMatspMat(LGraphT &, RGraphT &)":
    arguments:
      - description: Left matrix
        name: A
        type: LGraphT &
      - description: Right matrix
        name: B
        type: RGraphT &
    description: A*B
    return: edge_list containing matrix product
    signature_with_names: "template <typename ScalarT, typename LGraphT, typename RGraphT, typename MapOpT, typename ReduceOpT>\nedge_list<directed, ScalarT> spMatspMat(LGraphT & A, RGraphT & B)"
namespace:
  - nw
  - graph
---
