---
layout: library
title: back_edge_range.hpp
owner: Andrew Lumsdaine
brief: Range adapter for accessing back edges
tags:
  - sourcefile
library-type: sourcefile
---

```{index} back_edge_range.hpp
```
Given an edge i,j stores fast lookup to edge j,i. If edge j,i does not exist, store it in temporary data structure.