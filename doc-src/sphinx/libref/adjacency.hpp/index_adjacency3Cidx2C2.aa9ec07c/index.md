---
layout: class
title: index_adjacency<idx, index_type, vertex_id, Attributes>
owner: __MISSING__
brief: __MISSING__
tags:
  - class
defined_in_file: adjacency.hpp
declaration: "template <int idx, class index_type, class vertex_id, typename... Attributes>\nclass nw::graph::index_adjacency;"
dtor: unspecified
typedefs:
  attributes_t:
    definition: std::tuple<Attributes...>
    description: __MISSING__
  base:
    annotation:
      - private
    definition: indexed_struct_of_arrays<index_type, vertex_id, Attributes...>
    description: __MISSING__
  index_t:
    definition: index_type
    description: __MISSING__
  num_edges_type:
    definition: nw::graph::index_adjacency::index_t
    description: __MISSING__
  num_vertices_type:
    definition: std::array<vertex_id_type, 1>
    description: __MISSING__
  vertex_id_type:
    definition: vertex_id
    description: __MISSING__
namespace:
  - nw
  - graph
---

```{index}  index_adjacency<idx, index_type, vertex_id, Attributes>
```

