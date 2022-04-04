---
layout: method
title: operator()
owner: __MISSING__
brief: __MISSING__
tags:
  - method
defined_in_file: access.hpp
overloads:
  "template <class... args:auto>\nconstexpr auto operator()(auto &&...) const -> tag_invoke_result_t<nw::graph::num_edges_tag, decltype(args)...>":
    arguments:
      - description: __OPTIONAL__
        name: args
        type: auto &&...
    description: __OPTIONAL__
    return: __OPTIONAL__
    signature_with_names: "template <class... args:auto>\nconstexpr auto operator()(auto &&... args) const -> tag_invoke_result_t<nw::graph::num_edges_tag, decltype(args)...>"
---
