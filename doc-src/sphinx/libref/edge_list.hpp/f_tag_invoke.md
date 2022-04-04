---
layout: function
title: tag_invoke
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: edge_list.hpp
overloads:
  "template <class vertex_id, typename graph_base_t, nw::graph::directedness direct, typename... Attributes>\nauto tag_invoke(const nw::graph::num_edges_tag, const index_edge_list<vertex_id, graph_base_t, direct, Attributes...> &)":
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: const nw::graph::num_edges_tag
        unnamed: true
      - description: __OPTIONAL__
        name: b
        type: const index_edge_list<vertex_id, graph_base_t, direct, Attributes...> &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <class vertex_id, typename graph_base_t, nw::graph::directedness direct, typename... Attributes>\nauto tag_invoke(const nw::graph::num_edges_tag, const index_edge_list<vertex_id, graph_base_t, direct, Attributes...> & b)"
  "template <class vertex_id, typename graph_base_t, nw::graph::directedness direct, typename... Attributes>\nauto tag_invoke(const nw::graph::num_vertices_tag, const index_edge_list<vertex_id, graph_base_t, direct, Attributes...> &)":
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: const nw::graph::num_vertices_tag
        unnamed: true
      - description: __OPTIONAL__
        name: b
        type: const index_edge_list<vertex_id, graph_base_t, direct, Attributes...> &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <class vertex_id, typename graph_base_t, nw::graph::directedness direct, typename... Attributes>\nauto tag_invoke(const nw::graph::num_vertices_tag, const index_edge_list<vertex_id, graph_base_t, direct, Attributes...> & b)"
namespace:
  - nw
  - graph
---
