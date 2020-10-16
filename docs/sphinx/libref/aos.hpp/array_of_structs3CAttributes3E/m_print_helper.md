---
layout: method
title: print_helper
owner: __MISSING__
brief: __MISSING__
tags:
  - method
defined_in_file: aos.hpp
overloads:
  "template <size_t... Is>\nvoid print_helper(std::ostream &, std::tuple<Attributes...>, std::index_sequence<Is...>)":
    arguments:
      - description: __OPTIONAL__
        name: output_stream
        type: std::ostream &
      - description: __OPTIONAL__
        name: attrs
        type: std::tuple<Attributes...>
      - description: __OPTIONAL__
        name: unnamed-2
        type: std::index_sequence<Is...>
        unnamed: true
    description: __OPTIONAL__
    return: __OPTIONAL__
    signature_with_names: "template <size_t... Is>\nvoid print_helper(std::ostream & output_stream, std::tuple<Attributes...> attrs, std::index_sequence<Is...>)"
---
