---
layout: function
title: relaxed
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: util/atomic.hpp
overloads:
  "template <class T, class U>\nconstexpr void relaxed(T &&, U &&)":
    arguments:
      - description: __OPTIONAL__
        name: t
        type: T &&
      - description: __OPTIONAL__
        name: u
        type: U &&
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <class T, class U>\nconstexpr void relaxed(T && t, U && u)"
  "template <class T>\nconstexpr auto relaxed(T &&)":
    arguments:
      - description: __OPTIONAL__
        name: t
        type: T &&
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <class T>\nconstexpr auto relaxed(T && t)"
namespace:
  - nw
  - graph
---
