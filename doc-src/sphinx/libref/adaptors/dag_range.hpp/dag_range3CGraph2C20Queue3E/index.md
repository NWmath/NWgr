---
layout: class
title: dag_range<Graph, Queue>
owner: __MISSING__
brief: __MISSING__
tags:
  - class
defined_in_file: dag_range.hpp
declaration: "template <typename Graph, typename Queue>\nclass nw::graph::dag_range;"
dtor: unspecified
typedefs:
  VOV:
    definition: std::vector<std::vector<size_t>>
    description: __MISSING__
fields:
  Q_:
    annotation:
      - private
    description: __MISSING__
    type: Queue
  _pred_done_counter:
    annotation:
      - private
    description: __MISSING__
    type: std::vector<uint64_t>
  _predecessor_list:
    annotation:
      - private
    description: __MISSING__
    type: nw::graph::dag_range::VOV
  _ready_to_process:
    annotation:
      - private
    description: __MISSING__
    type: nw::graph::ready_to_process
  _successor_list:
    annotation:
      - private
    description: __MISSING__
    type: nw::graph::dag_range::VOV
  the_graph_:
    annotation:
      - private
    description: __MISSING__
    type: Graph &
namespace:
  - nw
  - graph
---

```{index}  dag_range<Graph, Queue>
```

