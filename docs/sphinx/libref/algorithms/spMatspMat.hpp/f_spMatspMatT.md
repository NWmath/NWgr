---
layout: function
title: spMatspMatT
owner: Andrew Lumsdaine, Scott McMillan
brief: A*B'
tags:
  - function
defined_in_file: algorithms/spMatspMat.hpp
overloads:
  "template <typename ScalarT, typename LGraphT, typename RGraphT, typename MapOpT, typename ReduceOpT>\nedge_list<directed, ScalarT> spMatspMatT(LGraphT &, RGraphT &)":
    arguments:
      - description: Left matrix
        name: A
        type: LGraphT &
      - description: Right matrix transpose
        name: BT
        type: RGraphT &
    description: A*B'
    return: edge_list containing matrix product
    signature_with_names: "template <typename ScalarT, typename LGraphT, typename RGraphT, typename MapOpT, typename ReduceOpT>\nedge_list<directed, ScalarT> spMatspMatT(LGraphT & A, RGraphT & BT)"
namespace:
  - nw
  - graph
---
