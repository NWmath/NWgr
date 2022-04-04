---
layout: function
title: cas
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: util/atomic.hpp
overloads:
  "template <std::memory_order success, std::memory_order failure, class T, class U, class V>\nconstexpr bool cas(T &&, U &&, V &&)":
    arguments:
      - description: __OPTIONAL__
        name: t
        type: T &&
      - description: __OPTIONAL__
        name: u
        type: U &&
      - description: __OPTIONAL__
        name: v
        type: V &&
    description: __OPTIONAL__
    return: __OPTIONAL__
    signature_with_names: "template <std::memory_order success, std::memory_order failure, class T, class U, class V>\nconstexpr bool cas(T && t, U && u, V && v)"
namespace:
  - nw
  - graph
---
