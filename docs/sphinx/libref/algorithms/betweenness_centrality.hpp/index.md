---
layout: library
title: algorithms/betweenness_centrality.hpp
owner: Andrew Lumsdaine, Kevin Deweese
brief: (Approximate) betweenness centrality
tags:
  - sourcefile
library-type: sourcefile
typedefs:
  vertex_id_type:
    definition: typename Graph::vertex_id_type
    description: __MISSING__
---

```{index}  algorithms/betweenness_centrality.hpp
```
Algorithm implementations for finding the betweeness centrality, which is typically defined for every vertex as the number of shortest paths in a graph passing through the vertex normalized by the number of all shortest paths for all pairs of vertices. Many of these algorithms use an approximate BC defined in the GAP benchmark suite, which does not use all pairs shortest paths, and instead only considers shortests paths connecting a small set of seed vertices.