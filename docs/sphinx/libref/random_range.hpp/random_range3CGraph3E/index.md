---
layout: class
title: random_range.hpp
owner: __MISSING__
brief: __MISSING__
tags:
  - class
defined_in_file: random_range.hpp
declaration: "template <typename Graph>\nclass nw::graph::random_range;"
dtor: unspecified
typedefs:
  iterator:
    definition: nw::graph::random_range::edge_range_iterator
    description: __MISSING__
fields:
  dice:
    annotation:
      - private
    description: __MISSING__
    type: decltype(std::bind(distribution, generator))
  distribution:
    annotation:
      - private
    description: __MISSING__
    type: std::uniform_real_distribution<double>
  generator:
    annotation:
      - private
    description: __MISSING__
    type: std::default_random_engine
  length_:
    annotation:
      - private
    description: __MISSING__
    type: size_t
  starting_vertex:
    annotation:
      - private
    description: __MISSING__
    type: nw::graph::vertex_id_t
  the_graph_:
    annotation:
      - private
    description: __MISSING__
    type: Graph &
namespace:
  - nw
  - graph
---
