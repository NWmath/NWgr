---
layout: class
title: tbbworklist_range2<Graph, Workitem, Queue>
owner: __MISSING__
brief: __MISSING__
tags:
  - class
defined_in_file: adaptors/worklist.hpp
declaration: "template <typename Graph, typename Workitem, typename Queue>\nclass nw::graph::tbbworklist_range2;"
dtor: unspecified
typedefs:
  iterator:
    definition: nw::graph::tbbworklist_range2::tbbworklist_range_iterator2
    description: __MISSING__
fields:
  buckets_:
    annotation:
      - private
    description: __MISSING__
    type: tbb::concurrent_vector<Queue>
  dummy_:
    annotation:
      - private
    description: __MISSING__
    type: Workitem
  the_graph_:
    annotation:
      - private
    description: __MISSING__
    type: Graph &
namespace:
  - nw
  - graph
---

```{index}  tbbworklist_range2<Graph, Workitem, Queue>
```

