---
layout: function
title: lower_bound_set_intersection
owner: Andrew Lumsdaine, Luke D'Alessandro
brief: Scan set intersection that uses std::lower_bound for head start
tags:
  - function
defined_in_file: algorithms/triangle_count.hpp
overloads:
  "template <typename _InputIterator1, typename _InputIterator2, typename _OutputIterator>\n_OutputIterator lower_bound_set_intersection(_InputIterator1, _InputIterator1, _InputIterator2, _InputIterator2, _OutputIterator)":
    arguments:
      - description: Beginning of the first range
        name: __first1
        type: _InputIterator1
      - description: End of the first range
        name: __last1
        type: _InputIterator1
      - description: Beginning of the second range
        name: __first2
        type: _InputIterator2
      - description: End of the second range
        name: __last2
        type: _InputIterator2
      - description: Accumulator output iterator
        name: __result
        type: _OutputIterator
    description: Compares the start index of each range, advances the beginning of the smaller index range to the beginning of the other range using std::lower_bound. Then intersect with a simple scan intersection.
    return: The number of set intersections
    signature_with_names: "template <typename _InputIterator1, typename _InputIterator2, typename _OutputIterator>\n_OutputIterator lower_bound_set_intersection(_InputIterator1 __first1, _InputIterator1 __last1, _InputIterator2 __first2, _InputIterator2 __last2, _OutputIterator __result)"
namespace:
  - nw
  - graph
---
