---
layout: class
title: compressed.hpp
owner: __MISSING__
brief: __MISSING__
tags:
  - class
defined_in_file: compressed.hpp
declaration: "template <std::size_t... Attrs>\nclass nw::graph::indexed_struct_of_arrays::edge_iterator;"
dtor: unspecified
typedefs:
  difference_type:
    definition: nw::graph::indexed_struct_of_arrays::edge_id_t
    description: __MISSING__
  iterator_category:
    definition: std::random_access_iterator_tag
    description: __MISSING__
  pointer:
    definition: nw::graph::select_t<std::tuple<vertex_id_t, const Attributes *...>, 0, 1, (Attrs + 2)...>
    description: __MISSING__
  reference:
    definition: nw::graph::select_t<std::tuple<vertex_id_t, const Attributes &...>, 0, 1, (Attrs + 2)...>
    description: __MISSING__
  value_type:
    definition: nw::graph::select_t<std::tuple<vertex_id_t, Attributes...>, 0, 1, (Attrs + 2)...>
    description: __MISSING__
fields:
  graph_:
    annotation:
      - private
    description: __MISSING__
    type: indexed_struct_of_arrays<Attributes...> &
  j_:
    annotation:
      - private
    description: __MISSING__
    type: nw::graph::indexed_struct_of_arrays::edge_id_t
  u_:
    annotation:
      - private
    description: __MISSING__
    type: nw::graph::vertex_id_t
namespace:
  - nw
  - graph
---
