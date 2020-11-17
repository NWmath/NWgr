---
layout: function
title: write_mm
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: mmio.hpp
overloads:
  "template <size_t w_idx, typename idxtype, int idx, typename... Attributes>\nvoid write_mm(const std::string &, adjacency<idx, Attributes...> &, const std::string &)":
    arguments:
      - description: __OPTIONAL__
        name: filename
        type: const std::string &
      - description: __OPTIONAL__
        name: A
        type: adjacency<idx, Attributes...> &
      - description: __OPTIONAL__
        name: file_symmetry
        type: const std::string &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <size_t w_idx, typename idxtype, int idx, typename... Attributes>\nvoid write_mm(const std::string & filename, adjacency<idx, Attributes...> & A, const std::string & file_symmetry)"
  "template <size_t w_idx, typename idxtype, nw::graph::directedness sym, typename... Attributes>\nvoid write_mm(const std::string &, edge_list<sym, Attributes...> &, const std::string &)":
    arguments:
      - description: __OPTIONAL__
        name: filename
        type: const std::string &
      - description: __OPTIONAL__
        name: A
        type: edge_list<sym, Attributes...> &
      - description: __OPTIONAL__
        name: file_symmetry
        type: const std::string &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <size_t w_idx, typename idxtype, nw::graph::directedness sym, typename... Attributes>\nvoid write_mm(const std::string & filename, edge_list<sym, Attributes...> & A, const std::string & file_symmetry)"
namespace:
  - nw
  - graph
---
