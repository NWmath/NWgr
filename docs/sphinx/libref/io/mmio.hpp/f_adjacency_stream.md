---
layout: function
title: adjacency_stream
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: io/mmio.hpp
overloads:
  "template <size_t w_idx, int idx, typename... Attributes>\nvoid adjacency_stream(std::ofstream &, adjacency<idx, Attributes...> &, const std::string &, std::string &)":
    arguments:
      - description: __OPTIONAL__
        name: outputStream
        type: std::ofstream &
      - description: __OPTIONAL__
        name: A
        type: adjacency<idx, Attributes...> &
      - description: __OPTIONAL__
        name: file_symmetry
        type: const std::string &
      - description: __OPTIONAL__
        name: w_type
        type: std::string &
    description: __OPTIONAL__
    return: __OPTIONAL__
    signature_with_names: "template <size_t w_idx, int idx, typename... Attributes>\nvoid adjacency_stream(std::ofstream & outputStream, adjacency<idx, Attributes...> & A, const std::string & file_symmetry, std::string & w_type)"
namespace:
  - nw
  - graph
---
