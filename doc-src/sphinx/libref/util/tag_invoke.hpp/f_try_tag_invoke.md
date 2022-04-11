---
layout: function
title: try_tag_invoke
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: util/tag_invoke.hpp
overloads:
  "template <typename CPO, typename... Args>\n_tag_invoke::no_type try_tag_invoke(, ...)":
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <typename CPO, typename... Args>\n_tag_invoke::no_type try_tag_invoke(, ...)"
  "template <typename CPO, typename... Args>\nauto try_tag_invoke(int) -> decltype(static_cast<void>(tag_invoke(std::declval<CPO>(), std::declval<Args>()...)) , _tag_invoke::yes_type{})":
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: int
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <typename CPO, typename... Args>\nauto try_tag_invoke(int) -> decltype(static_cast<void>(tag_invoke(std::declval<CPO>(), std::declval<Args>()...)) , _tag_invoke::yes_type{})"
namespace:
  - _tag_invoke
---
