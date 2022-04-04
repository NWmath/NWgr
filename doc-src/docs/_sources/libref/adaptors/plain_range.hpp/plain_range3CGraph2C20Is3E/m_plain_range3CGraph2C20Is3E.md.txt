---
layout: method
title: plain_range<Graph, Is...>
owner: __MISSING__
brief: __MISSING__
tags:
  - method
defined_in_file: adaptors/plain_range.hpp
is_ctor: true
overloads:
  plain_range<Graph, Is...>(Graph &):
    arguments:
      - description: __OPTIONAL__
        name: g
        type: Graph &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: plain_range<Graph, Is...>(Graph & g)
  plain_range<Graph, Is...>(Graph &, std::size_t):
    arguments:
      - description: __OPTIONAL__
        name: g
        type: Graph &
      - description: __OPTIONAL__
        name: offset
        type: std::size_t
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: plain_range<Graph, Is...>(Graph & g, std::size_t offset)
  plain_range<Graph, Is...>(const plain_range<Graph, Is...> &):
    annotation:
      - default
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: const plain_range<Graph, Is...> &
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: plain_range<Graph, Is...>(const plain_range<Graph, Is...> &)
  plain_range<Graph, Is...>(plain_range<Graph, Is...> &, tbb::split):
    arguments:
      - description: __OPTIONAL__
        name: b
        type: plain_range<Graph, Is...> &
      - description: __OPTIONAL__
        name: unnamed-1
        type: tbb::split
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: plain_range<Graph, Is...>(plain_range<Graph, Is...> & b, tbb::split)
---
