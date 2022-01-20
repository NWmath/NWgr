---
layout: method
title: index_adjacency<idx, index_type, vertex_id, Attributes...>
owner: __MISSING__
brief: __MISSING__
tags:
  - method
defined_in_file: adjacency.hpp
is_ctor: true
overloads:
  index_adjacency<idx, index_type, vertex_id, Attributes...>(size_t, size_t):
    arguments:
      - description: __OPTIONAL__
        name: N
        type: size_t
      - description: __OPTIONAL__
        name: M
        type: size_t
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: index_adjacency<idx, index_type, vertex_id, Attributes...>(size_t N, size_t M)
  index_adjacency<idx, index_type, vertex_id, Attributes...>(std::array<size_t, 1>, size_t):
    arguments:
      - description: __OPTIONAL__
        name: N
        type: std::array<size_t, 1>
      - description: __OPTIONAL__
        name: M
        type: size_t
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: index_adjacency<idx, index_type, vertex_id, Attributes...>(std::array<size_t, 1> N, size_t M)
  "template <class ExecutionPolicy>\nindex_adjacency<idx, index_type, vertex_id, Attributes...>(index_edge_list<nw::graph::index_adjacency::vertex_id_type, nw::graph::unipartite_graph_base, directedness::directed, Attributes...> &, ExecutionPolicy &&)":
    arguments:
      - description: __OPTIONAL__
        name: A
        type: index_edge_list<nw::graph::index_adjacency::vertex_id_type, nw::graph::unipartite_graph_base, directedness::directed, Attributes...> &
      - description: __OPTIONAL__
        name: policy
        type: ExecutionPolicy &&
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <class ExecutionPolicy>\nindex_adjacency<idx, index_type, vertex_id, Attributes...>(index_edge_list<nw::graph::index_adjacency::vertex_id_type, nw::graph::unipartite_graph_base, directedness::directed, Attributes...> & A, ExecutionPolicy && policy)"
  "template <class ExecutionPolicy>\nindex_adjacency<idx, index_type, vertex_id, Attributes...>(index_edge_list<nw::graph::index_adjacency::vertex_id_type, nw::graph::unipartite_graph_base, directedness::undirected, Attributes...> &, ExecutionPolicy &&)":
    arguments:
      - description: __OPTIONAL__
        name: A
        type: index_edge_list<nw::graph::index_adjacency::vertex_id_type, nw::graph::unipartite_graph_base, directedness::undirected, Attributes...> &
      - description: __OPTIONAL__
        name: policy
        type: ExecutionPolicy &&
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <class ExecutionPolicy>\nindex_adjacency<idx, index_type, vertex_id, Attributes...>(index_edge_list<nw::graph::index_adjacency::vertex_id_type, nw::graph::unipartite_graph_base, directedness::undirected, Attributes...> & A, ExecutionPolicy && policy)"
---
