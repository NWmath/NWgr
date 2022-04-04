---
layout: method
title: adjacency<idx, Attributes...>
owner: __MISSING__
brief: __MISSING__
tags:
  - method
defined_in_file: compressed.hpp
is_ctor: true
overloads:
  adjacency<idx, Attributes...>(edge_list<directed, Attributes...> &):
    arguments:
      - description: __OPTIONAL__
        name: A
        type: edge_list<directed, Attributes...> &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: adjacency<idx, Attributes...>(edge_list<directed, Attributes...> & A)
  adjacency<idx, Attributes...>(edge_list<undirected, Attributes...> &):
    arguments:
      - description: __OPTIONAL__
        name: A
        type: edge_list<undirected, Attributes...> &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: adjacency<idx, Attributes...>(edge_list<undirected, Attributes...> & A)
  adjacency<idx, Attributes...>(size_t, size_t):
    arguments:
      - description: __OPTIONAL__
        name: N
        type: size_t
      - description: __OPTIONAL__
        name: M
        type: size_t
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: adjacency<idx, Attributes...>(size_t N, size_t M)
---
