---
layout: library
title: csc.hpp
owner: Andrew Lumsdaine
brief: Compressed Sparse Column Storage
tags:
  - sourcefile
library-type: sourcefile
typedefs:
  csc_graph:
    definition: adjacency<1>
    description: CSC Graph
  csc_matrix:
    definition: adjacency<1, T>
    description: CSC Matrix
---

This is an alias for the `adjacency` structure, compressed along the `1` axis.
The `csc_graph` stores two indices while the `csc_matrix` also stores a value.
The type of the value is parameterized and defaults to `double.`

```{index}  csc.hpp
```

