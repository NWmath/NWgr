---
layout: class
title: tbbworklist_range<Graph, Workitem, Queue>
owner: __MISSING__
brief: __MISSING__
tags:
  - class
defined_in_file: adaptors/worklist.hpp
declaration: "template <typename Graph, typename Workitem, typename Queue>\nclass nw::graph::tbbworklist_range;"
dtor: unspecified
typedefs:
  iterator:
    definition: nw::graph::tbbworklist_range::tbbworklist_range_iterator
    description: __MISSING__
fields:
  Q_:
    annotation:
      - private
    description: __MISSING__
    type: Queue
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

```{index}  tbbworklist_range<Graph, Workitem, Queue>
```

