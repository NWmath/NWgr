---
layout: function
title: spMatspMatT
owner: Andrew Lumsdaine, Scott McMillan
brief: A*B'
tags:
  - function
defined_in_file: algorithms/spMatspMat.hpp
overloads:
  "template <typename ScalarT, typename LGraphT, typename RGraphT, typename MapOpT, typename ReduceOpT>\nedge_list<directedness::directed, ScalarT> spMatspMatT(LGraphT &, RGraphT &)":
    arguments:
      - description: __OPTIONAL__
        name: A
        type: LGraphT &
      - description: __OPTIONAL__
        name: BT
        type: RGraphT &
    description: __OPTIONAL__
    return: __OPTIONAL__
    signature_with_names: "template <typename ScalarT, typename LGraphT, typename RGraphT, typename MapOpT, typename ReduceOpT>\nedge_list<directedness::directed, ScalarT> spMatspMatT(LGraphT & A, RGraphT & BT)"
namespace:
  - nw
  - graph
---
