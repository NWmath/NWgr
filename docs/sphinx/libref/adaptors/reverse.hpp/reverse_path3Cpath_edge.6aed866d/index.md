---
layout: class
title: reverse_path<path_edge, vertex_id>
owner: __MISSING__
brief: __MISSING__
tags:
  - class
defined_in_file: adaptors/reverse.hpp
declaration: "template <typename path_edge, class vertex_id>\nclass nw::graph::reverse_path;"
dtor: unspecified
typedefs:
  vertex_id_type:
    definition: vertex_id
    description: __MISSING__
fields:
  done:
    description: __MISSING__
    type: bool
  path_:
    description: __MISSING__
    type: std::vector<path_edge>
  start_:
    description: __MISSING__
    type: nw::graph::reverse_path::vertex_id_type
  stop_:
    description: __MISSING__
    type: nw::graph::reverse_path::vertex_id_type
  update_:
    description: __MISSING__
    type: double
namespace:
  - nw
  - graph
---
