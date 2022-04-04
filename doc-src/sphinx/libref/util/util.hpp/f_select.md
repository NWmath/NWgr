---
layout: function
title: select
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: util/util.hpp
overloads:
  "template <std::size_t... Is, class Tuple, class >\nconstexpr auto select(Tuple &&) -> std::tuple<std::tuple_element_t<Is, std::decay_t<Tuple>>...>":
    arguments:
      - description: __OPTIONAL__
        name: t
        type: Tuple &&
    description: __OPTIONAL__
    return: __OPTIONAL__
    signature_with_names: "template <std::size_t... Is, class Tuple, class >\nconstexpr auto select(Tuple && t) -> std::tuple<std::tuple_element_t<Is, std::decay_t<Tuple>>...>"
namespace:
  - nw
  - graph
---
