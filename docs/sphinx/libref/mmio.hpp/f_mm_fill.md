---
layout: function
title: mm_fill
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: mmio.hpp
overloads:
  "template <typename T>\nvoid mm_fill(std::istream &, edge_list<directed, T> &, size_t, bool, bool)":
    arguments:
      - description: __OPTIONAL__
        name: inputStream
        type: std::istream &
      - description: __OPTIONAL__
        name: A
        type: edge_list<directed, T> &
      - description: __OPTIONAL__
        name: nNonzeros
        type: size_t
      - description: __OPTIONAL__
        name: file_symmetry
        type: bool
      - description: __OPTIONAL__
        name: pattern
        type: bool
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <typename T>\nvoid mm_fill(std::istream & inputStream, edge_list<directed, T> & A, size_t nNonzeros, bool file_symmetry, bool pattern)"
  "template <typename T>\nvoid mm_fill(std::istream &, edge_list<undirected, T> &, size_t, bool, bool)":
    arguments:
      - description: __OPTIONAL__
        name: inputStream
        type: std::istream &
      - description: __OPTIONAL__
        name: A
        type: edge_list<undirected, T> &
      - description: __OPTIONAL__
        name: nNonzeros
        type: size_t
      - description: __OPTIONAL__
        name: file_symmetry
        type: bool
      - description: __OPTIONAL__
        name: pattern
        type: bool
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <typename T>\nvoid mm_fill(std::istream & inputStream, edge_list<undirected, T> & A, size_t nNonzeros, bool file_symmetry, bool pattern)"
  void mm_fill(std::istream &, edge_list<directed> &, size_t, bool, bool):
    arguments:
      - description: __OPTIONAL__
        name: inputStream
        type: std::istream &
      - description: __OPTIONAL__
        name: A
        type: edge_list<directed> &
      - description: __OPTIONAL__
        name: nNonzeros
        type: size_t
      - description: __OPTIONAL__
        name: file_symmetry
        type: bool
      - description: __OPTIONAL__
        name: pattern
        type: bool
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: void mm_fill(std::istream & inputStream, edge_list<directed> & A, size_t nNonzeros, bool file_symmetry, bool pattern)
  void mm_fill(std::istream &, edge_list<undirected> &, size_t, bool, bool):
    arguments:
      - description: __OPTIONAL__
        name: inputStream
        type: std::istream &
      - description: __OPTIONAL__
        name: A
        type: edge_list<undirected> &
      - description: __OPTIONAL__
        name: nNonzeros
        type: size_t
      - description: __OPTIONAL__
        name: file_symmetry
        type: bool
      - description: __OPTIONAL__
        name: pattern
        type: bool
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: void mm_fill(std::istream & inputStream, edge_list<undirected> & A, size_t nNonzeros, bool file_symmetry, bool pattern)
namespace:
  - nw
  - graph
---
