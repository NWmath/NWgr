---
layout: method
title: fill
owner: __MISSING__
brief: __MISSING__
tags:
  - method
defined_in_file: edge_list.hpp
overloads:
  "template <int idx, class ExecutionPolicy>\nvoid fill(adjacency<idx, Attributes...> &, ExecutionPolicy &&)":
    arguments:
      - description: __OPTIONAL__
        name: cs
        type: adjacency<idx, Attributes...> &
      - description: __OPTIONAL__
        name: policy
        type: ExecutionPolicy &&
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <int idx, class ExecutionPolicy>\nvoid fill(adjacency<idx, Attributes...> & cs, ExecutionPolicy && policy)"
  "template <int idx, nw::graph::directedness sym>\nvoid fill(compressed_sparse<idx, sym, Attributes...> &)":
    arguments:
      - description: __OPTIONAL__
        name: cs
        type: compressed_sparse<idx, sym, Attributes...> &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <int idx, nw::graph::directedness sym>\nvoid fill(compressed_sparse<idx, sym, Attributes...> & cs)"
  "template <int idx, nw::graph::succession cessor>\nvoid fill(packed<idx, cessor, Attributes...> &)":
    arguments:
      - description: __OPTIONAL__
        name: cs
        type: packed<idx, cessor, Attributes...> &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <int idx, nw::graph::succession cessor>\nvoid fill(packed<idx, cessor, Attributes...> & cs)"
  void fill(adj_list<Attributes...> &):
    arguments:
      - description: __OPTIONAL__
        name: al
        type: adj_list<Attributes...> &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: void fill(adj_list<Attributes...> & al)
---
