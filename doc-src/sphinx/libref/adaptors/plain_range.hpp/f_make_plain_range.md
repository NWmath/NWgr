---
layout: function
title: make_plain_range
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: adaptors/plain_range.hpp
overloads:
  "template <class Graph, std::size_t... Is>\nstatic plain_range<Graph, Is...> make_plain_range(Graph &)":
    arguments:
      - description: __OPTIONAL__
        name: g
        type: Graph &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <class Graph, std::size_t... Is>\nstatic plain_range<Graph, Is...> make_plain_range(Graph & g)"
  "template <class Graph, std::size_t... Is>\nstatic plain_range<Graph, Is...> make_plain_range(Graph &, std::size_t)":
    arguments:
      - description: __OPTIONAL__
        name: g
        type: Graph &
      - description: __OPTIONAL__
        name: offset
        type: std::size_t
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <class Graph, std::size_t... Is>\nstatic plain_range<Graph, Is...> make_plain_range(Graph & g, std::size_t offset)"
namespace:
  - nw
  - graph
---