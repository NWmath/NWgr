---
layout: method
title: operator()
owner: __MISSING__
brief: __MISSING__
tags:
  - method
defined_in_file: util/tag_invoke.hpp
overloads:
  "template <typename CPO, typename... Args>\nconstexpr auto operator()(CPO, Args &&...) const -> decltype(tag_invoke((CPO &&)cpo, (Args &&)args...))":
    arguments:
      - description: __OPTIONAL__
        name: cpo
        type: CPO
      - description: __OPTIONAL__
        name: args
        type: Args &&...
    description: __OPTIONAL__
    return: __OPTIONAL__
    signature_with_names: "template <typename CPO, typename... Args>\nconstexpr auto operator()(CPO cpo, Args &&... args) const -> decltype(tag_invoke((CPO &&)cpo, (Args &&)args...))"
---
