---
layout: method
title: edge_range<Graph, Is...>
owner: __MISSING__
brief: __MISSING__
tags:
  - method
defined_in_file: edge_range.hpp
is_ctor: true
overloads:
  edge_range<Graph, Is...>(Graph &, std::index_sequence<Is...>):
    arguments:
      - description: __OPTIONAL__
        name: g
        type: Graph &
      - description: __OPTIONAL__
        name: is
        type: std::index_sequence<Is...>
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: edge_range<Graph, Is...>(Graph & g, std::index_sequence<Is...> is)
  edge_range<Graph, Is...>(Graph &, std::size_t, std::index_sequence<Is...>):
    arguments:
      - description: __OPTIONAL__
        name: g
        type: Graph &
      - description: __OPTIONAL__
        name: offset
        type: std::size_t
      - description: __OPTIONAL__
        name: unnamed-2
        type: std::index_sequence<Is...>
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: edge_range<Graph, Is...>(Graph & g, std::size_t offset, std::index_sequence<Is...>)
  edge_range<Graph, Is...>(const edge_range<Graph, Is...> &):
    annotation:
      - default
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: const edge_range<Graph, Is...> &
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: edge_range<Graph, Is...>(const edge_range<Graph, Is...> &)
  edge_range<Graph, Is...>(edge_range<Graph, Is...> &, tbb::split):
    arguments:
      - description: __OPTIONAL__
        name: b
        type: edge_range<Graph, Is...> &
      - description: __OPTIONAL__
        name: unnamed-1
        type: tbb::split
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: edge_range<Graph, Is...>(edge_range<Graph, Is...> & b, tbb::split)
---
