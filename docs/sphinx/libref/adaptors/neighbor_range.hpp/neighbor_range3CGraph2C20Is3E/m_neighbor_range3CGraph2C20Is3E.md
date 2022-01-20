---
layout: method
title: neighbor_range<Graph, Is...>
owner: __MISSING__
brief: __MISSING__
tags:
  - method
defined_in_file: adaptors/neighbor_range.hpp
is_ctor: true
overloads:
  neighbor_range<Graph, Is...>(Graph &):
    arguments:
      - description: __OPTIONAL__
        name: g
        type: Graph &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: neighbor_range<Graph, Is...>(Graph & g)
  neighbor_range<Graph, Is...>(Graph &, std::size_t):
    arguments:
      - description: __OPTIONAL__
        name: g
        type: Graph &
      - description: __OPTIONAL__
        name: offset
        type: std::size_t
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: neighbor_range<Graph, Is...>(Graph & g, std::size_t offset)
  neighbor_range<Graph, Is...>(const neighbor_range<Graph, Is...> &):
    annotation:
      - default
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: const neighbor_range<Graph, Is...> &
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: neighbor_range<Graph, Is...>(const neighbor_range<Graph, Is...> &)
  neighbor_range<Graph, Is...>(neighbor_range<Graph, Is...> &, tbb::split):
    arguments:
      - description: __OPTIONAL__
        name: b
        type: neighbor_range<Graph, Is...> &
      - description: __OPTIONAL__
        name: unnamed-1
        type: tbb::split
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: neighbor_range<Graph, Is...>(neighbor_range<Graph, Is...> & b, tbb::split)
---
