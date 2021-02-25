---
layout: library
title: coo.hpp
owner: Andrew Lumsdaine
brief: Coordinate Format Sparse Storage
tags:
  - sourcefile
library-type: sourcefile
typedefs:
  coo_graph:
    definition: edge_list<directedness::directed>
    description: COO Graph
  coo_matrix:
    definition: edge_list<directedness::directed, T>
    description: COO Sparse Matrix
---

This is an alias for an `edge_list.`  
The `coo_graph` just represents a pair of indices, whereas 
the `coo_matrix` also represents an item to be stored.

```{index}  coo.hpp
```

