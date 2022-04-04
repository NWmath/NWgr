---
layout: library
title: csr.hpp
owner: Andrew Lumsdaine
brief: Compressed Sparse Row Storage Format
tags:
  - sourcefile
library-type: sourcefile
typedefs:
  csr_graph:
    definition: adjacency<0>
    description: CSR Graph
  csr_matrix:
    definition: adjacency<0, T>
    description: CSR Sparse Matrix
---

This is an alias for the `adjacency` structure, compressed along the `0` axis.
The `csr_graph` stores two indices while the `csr_matrix` also stores a value.
The type of the value is parameterized and defaults to `double.`

```{index}  csr.hpp
```

