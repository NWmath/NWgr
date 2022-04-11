---
layout: library
title: algorithms/page_rank.hpp
owner: Andrew Lumsdaine, Scott McMillan
brief: Page rank
tags:
  - sourcefile
library-type: sourcefile
typedefs:
  counting_iterator:
    definition: tbb::counting_iterator<T>
    description: __MISSING__
  vertex_id_type:
    definition: typename Graph::vertex_id_type
    description: __MISSING__
---

```{index}  algorithms/page_rank.hpp
```
Page rank estimates the importance of vertices in a graph by the probability that a graph traversal will end up at a specfic vertex during a random walk.