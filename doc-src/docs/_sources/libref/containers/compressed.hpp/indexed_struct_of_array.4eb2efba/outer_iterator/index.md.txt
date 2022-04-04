---
layout: class
title: outer_iterator
owner: __MISSING__
brief: __MISSING__
tags:
  - class
defined_in_file: compressed.hpp
declaration: "\nclass nw::graph::indexed_struct_of_arrays::outer_iterator;"
dtor: unspecified
typedefs:
  difference_type:
    definition: nw::graph::vertex_id_t
    description: __MISSING__
  iterator_category:
    definition: std::random_access_iterator_tag
    description: __MISSING__
  pointer:
    definition: nw::graph::indexed_struct_of_arrays::outer_iterator::value_type *
    description: __MISSING__
  reference:
    definition: nw::graph::indexed_struct_of_arrays::outer_iterator::value_type &
    description: __MISSING__
  value_type:
    definition: nw::graph::indexed_struct_of_arrays::sub_view
    description: __MISSING__
fields:
  i_:
    annotation:
      - private
    description: __MISSING__
    type: nw::graph::vertex_id_t
  indexed_:
    annotation:
      - private
    description: __MISSING__
    type: typename struct_of_arrays<Attributes...>::iterator
  indices_:
    annotation:
      - private
    description: __MISSING__
    type: std::vector<vertex_id_t>::iterator
namespace:
  - nw
  - graph
---

```{index}  outer_iterator
```

