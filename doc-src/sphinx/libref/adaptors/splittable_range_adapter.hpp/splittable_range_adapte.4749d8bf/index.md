---
layout: class
title: splittable_range_adapter<Iterator>
owner: __MISSING__
brief: __MISSING__
tags:
  - class
defined_in_file: adaptors/splittable_range_adapter.hpp
declaration: "template <class Iterator>\nclass nw::graph::splittable_range_adapter;"
dtor: unspecified
typedefs:
  iterator:
    definition: Iterator
    description: __MISSING__
  value_type:
    definition: typename iterator::value_type
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
    type: std::size_t
  end_:
    annotation:
      - private
    description: __MISSING__
    type: Iterator
namespace:
  - nw
  - graph
---

```{index}  splittable_range_adapter<Iterator>
```

