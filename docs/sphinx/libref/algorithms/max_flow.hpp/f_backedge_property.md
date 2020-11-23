---
layout: function
title: backedge_property
owner: Andrew Lumsdaine, Kevin Deweese
brief: Back edge property
tags:
  - function
defined_in_file: algorithms/max_flow.hpp
overloads:
  "template <size_t Idx, typename Edge>\nauto backedge_property(Edge)":
    arguments:
      - description: Incident edge
        name: edge
        type: Edge
    description: Selects a desired property from edge tuple.
    return: Edge property value
    signature_with_names: "template <size_t Idx, typename Edge>\nauto backedge_property(Edge edge)"
namespace:
  - nw
  - graph
---
