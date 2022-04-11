---
layout: class
title: timer<D>
owner: __MISSING__
brief: __MISSING__
tags:
  - class
defined_in_file: util/timer.hpp
declaration: "template <class D>\nclass nw::util::timer;"
dtor: unspecified
typedefs:
  time_t:
    annotation:
      - private
    definition: std::chrono::time_point<std::chrono::system_clock>
    description: __MISSING__
fields:
  msg_:
    annotation:
      - protected
    description: __MISSING__
    type: std::string
  start_time:
    annotation:
      - private
    description: __MISSING__
    type: nw::util::timer::time_t
  stop_time:
    annotation:
      - private
    description: __MISSING__
    type: nw::util::timer::time_t
namespace:
  - nw
  - util
---

```{index}  timer<D>
```

