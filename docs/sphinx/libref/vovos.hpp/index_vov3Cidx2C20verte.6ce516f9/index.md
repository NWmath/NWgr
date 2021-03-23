---
layout: class
title: index_vov<idx, vertex_id, Attributes>
owner: __MISSING__
brief: __MISSING__
tags:
  - class
defined_in_file: vovos.hpp
declaration: "template <int idx, class vertex_id, typename... Attributes>\nclass nw::graph::index_vov;"
dtor: unspecified
typedefs:
  attributes_t:
    definition: std::tuple<Attributes...>
    description: __MISSING__
  base:
    annotation:
      - private
    definition: vector_of_vector_of_structs<vertex_id, Attributes...>
    description: __MISSING__
  graph_base:
    definition: nw::graph::unipartite_graph_base
    description: __MISSING__
  num_edges_type:
    definition: typename base::size_type
    description: __MISSING__
  num_vertices_type:
    definition: std::array<typename base::size_type, 1>
    description: __MISSING__
  vertex_id_type:
    definition: vertex_id
    description: __MISSING__
fields:
  num_edges_:
    annotation:
      - private
    description: __MISSING__
    type: nw::graph::index_vov::num_edges_type
namespace:
  - nw
  - graph
---

```{index}  index_vov<idx, vertex_id, Attributes>
```

