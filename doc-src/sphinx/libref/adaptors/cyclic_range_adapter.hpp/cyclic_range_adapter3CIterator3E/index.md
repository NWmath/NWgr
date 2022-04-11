---
layout: class
title: cyclic_range_adapter<Iterator>
owner: __MISSING__
brief: __MISSING__
tags:
  - class
defined_in_file: adaptors/cyclic_range_adapter.hpp
declaration: "template <class Iterator>\nclass nw::graph::cyclic_range_adapter;"
dtor: unspecified
typedefs:
  difference_type:
    definition: typename std::iterator_traits<Iterator>::difference_type
    description: __MISSING__
fields:
  begin_:
    annotation:
      - private
    description: __MISSING__
    type: Iterator
  cutoff_:
    annotation:
      - private
    description: __MISSING__
    type: nw::graph::cyclic_range_adapter::difference_type
  cycle_:
    annotation:
      - private
    description: __MISSING__
    type: nw::graph::cyclic_range_adapter::difference_type
  end_:
    annotation:
      - private
    description: __MISSING__
    type: Iterator
  stride_:
    annotation:
      - private
    description: __MISSING__
    type: nw::graph::cyclic_range_adapter::difference_type
namespace:
  - nw
  - graph
---

```{index}  cyclic_range_adapter<Iterator>
```

