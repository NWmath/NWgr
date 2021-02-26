---
layout: function
title: tag_invoke
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: vofos.hpp
overloads:
  "template <int idx, class vertex_id, typename... Attributes>\nauto tag_invoke(const nw::graph::num_vertices_tag, index_adj_flist<idx, vertex_id, Attributes...> &)":
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: const nw::graph::num_vertices_tag
        unnamed: true
      - description: __OPTIONAL__
        name: b
        type: index_adj_flist<idx, vertex_id, Attributes...> &
    description: __OPTIONAL__
    return: __OPTIONAL__
    signature_with_names: "template <int idx, class vertex_id, typename... Attributes>\nauto tag_invoke(const nw::graph::num_vertices_tag, index_adj_flist<idx, vertex_id, Attributes...> & b)"
namespace:
  - nw
  - graph
---
