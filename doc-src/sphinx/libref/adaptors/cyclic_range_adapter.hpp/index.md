---
layout: library
title: adaptors/cyclic_range_adapter.hpp
owner: Luke D'Alessandro
brief: A range adapter for splitting an underlying range into cycles for TBB
tags:
  - sourcefile
library-type: sourcefile
---

```{index} cyclic_range_adapter.hpp
```

This adapter takes an underlying random access range and provides the
ability to split the range into cycles for TBB. A cycle is a subset of the
range such that each subsequent element is some stride from the previous
element.

The cyclic range adapter is implemented recursively, that is that each time
the range is split it simply returns two ranges that cover the previous
range, each with twice the cycle and one offset by one element.

Key to the adapter is the _cutoff_, which is defined in terms of the maximum
stride rather than in terms of the number of elements. A _cutoff_ of `1`
implies that the range can't be split, while a `_cutoff_` of `n` means that
the range can be split into up to `n` cycles.
