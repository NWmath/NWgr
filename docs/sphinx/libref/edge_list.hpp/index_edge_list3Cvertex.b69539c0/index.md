---
layout: class
title: index_edge_list<vertex_id, graph_base_t, direct, Attributes>
owner: __MISSING__
brief: __MISSING__
tags:
  - class
defined_in_file: edge_list.hpp
declaration: "template <class vertex_id, typename graph_base_t, nw::graph::directedness direct, typename... Attributes>\nclass nw::graph::index_edge_list;"
dtor: unspecified
typedefs:
  attributes_t:
    definition: std::tuple<Attributes...>
    description: __MISSING__
  base:
    definition: struct_of_arrays<nw::graph::index_edge_list::vertex_id_type, nw::graph::index_edge_list::vertex_id_type, Attributes...>
    description: __MISSING__
  directed_type:
    definition: index_edge_list<nw::graph::index_edge_list::vertex_id_type, graph_base_t, directedness::directed, Attributes...>
    description: __MISSING__
  element:
    definition: std::tuple<vertex_id_type, vertex_id_type, Attributes...>
    description: __MISSING__
  graph_base:
    definition: graph_base_t
    description: __MISSING__
  my_type:
    definition: index_edge_list<nw::graph::index_edge_list::vertex_id_type, graph_base_t, direct, Attributes...>
    description: __MISSING__
  num_edges_type:
    definition: typename base::difference_type
    description: __MISSING__
  num_vertices_type:
    definition: typename graph_base::vertex_cardinality_t
    description: __MISSING__
  undirected_type:
    definition: index_edge_list<nw::graph::index_edge_list::vertex_id_type, graph_base_t, directedness::undirected, Attributes...>
    description: __MISSING__
  vertex_id_type:
    definition: vertex_id
    description: __MISSING__
fields:
  edge_directedness:
    description: __MISSING__
    type: const nw::graph::directedness
  is_unipartite:
    description: __MISSING__
    type: const bool
  magic:
    description: __MISSING__
    type: const char const[27]
namespace:
  - nw
  - graph
---

```{index}  index_edge_list<vertex_id, graph_base_t, direct, Attributes>
```

