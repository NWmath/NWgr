---
layout: function
title: tag_invoke
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: adjacency.hpp
overloads:
  "template <int idx, class index_type, class vertex_id_type, class lookup_type, typename... Attributes>\nauto tag_invoke(const nw::graph::degree_tag, const index_adjacency<idx, index_type, vertex_id_type, Attributes...> &, lookup_type)":
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: const nw::graph::degree_tag
        unnamed: true
      - description: __OPTIONAL__
        name: g
        type: const index_adjacency<idx, index_type, vertex_id_type, Attributes...> &
      - description: __OPTIONAL__
        name: i
        type: lookup_type
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <int idx, class index_type, class vertex_id_type, class lookup_type, typename... Attributes>\nauto tag_invoke(const nw::graph::degree_tag, const index_adjacency<idx, index_type, vertex_id_type, Attributes...> & g, lookup_type i)"
  "template <int idx, class index_type, class vertex_id_type, typename... Attributes>\nauto tag_invoke(const nw::graph::num_vertices_tag, const index_adjacency<idx, index_type, vertex_id_type, Attributes...> &)":
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: const nw::graph::num_vertices_tag
        unnamed: true
      - description: __OPTIONAL__
        name: g
        type: const index_adjacency<idx, index_type, vertex_id_type, Attributes...> &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <int idx, class index_type, class vertex_id_type, typename... Attributes>\nauto tag_invoke(const nw::graph::num_vertices_tag, const index_adjacency<idx, index_type, vertex_id_type, Attributes...> & g)"
namespace:
  - nw
  - graph
---
