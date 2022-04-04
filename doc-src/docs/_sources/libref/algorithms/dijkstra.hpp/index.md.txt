---
layout: library
title: algorithms/dijkstra.hpp
owner: Andrew Lumsdaine
brief: Dijkstra's algorithm
tags:
  - sourcefile
library-type: sourcefile
typedefs:
  vertex_id_type:
    definition: vertex_id_t<GraphT>
    description: __MISSING__
  weight_t:
    definition: DistanceT
    description: __MISSING__
  weighted_vertex:
    definition: std::tuple<vertex_id_type, weight_t>
    description: __MISSING__
---

```{index}  algorithms/dijkstra.hpp
```
Dijkstra's algorithm for finding single source shortest path.
