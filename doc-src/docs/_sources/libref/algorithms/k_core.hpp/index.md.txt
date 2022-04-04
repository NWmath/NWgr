---
layout: library
title: algorithms/k_core.hpp
owner: Andrew Lumsdaine, Kevin Deweese
brief: K-core
tags:
  - sourcefile
library-type: sourcefile
typedefs:
  Neighbors:
    definition: std::pair<size_t, size_t>
    description: Neighbor pairs
  Unordered_map:
    definition: std::unordered_map<Neighbors, bool, pair_hash>
    description: Map indicating whether an edge pair remains in the k-core (false) or has been filtered (true)
---

```{index}  algorithms/k_core.hpp
```
Find the k-core, the induced subgraph containing vertices with degree >= k.
