---
layout: library
title: graph_concepts.hpp
owner: Andrew Lumsdaine
brief: Initial concepts for NW Graph algorithms
tags:
  - sourcefile
library-type: sourcefile
typedefs:
  inner_range:
    definition: typename std::iterator_traits<typename G::iterator>::value_type
    description: __MISSING__
  inner_value:
    definition: typename std::iterator_traits<typename inner_range<G>::iterator>::value_type
    description: __MISSING__
---

```{index}  graph_concepts.hpp
```

