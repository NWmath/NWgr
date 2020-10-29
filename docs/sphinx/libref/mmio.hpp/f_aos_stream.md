---
layout: function
title: aos_stream
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: mmio.hpp
overloads:
  "template <size_t w_idx, nw::graph::directedness sym, typename... Attributes>\nvoid aos_stream(std::ofstream &, edge_list<sym, Attributes...>, const std::string &, std::string &)":
    arguments:
      - description: __OPTIONAL__
        name: outputStream
        type: std::ofstream &
      - description: __OPTIONAL__
        name: A
        type: edge_list<sym, Attributes...>
      - description: __OPTIONAL__
        name: file_symmetry
        type: const std::string &
      - description: __OPTIONAL__
        name: w_type
        type: std::string &
    description: __OPTIONAL__
    return: __OPTIONAL__
    signature_with_names: "template <size_t w_idx, nw::graph::directedness sym, typename... Attributes>\nvoid aos_stream(std::ofstream & outputStream, edge_list<sym, Attributes...> A, const std::string & file_symmetry, std::string & w_type)"
namespace:
  - nw
  - graph
---
