---
layout: function
title: mm_fill
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: io/mmio.hpp
overloads:
  "template <typename T>\nvoid mm_fill(std::istream &, edge_list<directedness::directed, T> &, size_t, bool, bool)":
    arguments:
      - description: __OPTIONAL__
        name: inputStream
        type: std::istream &
      - description: __OPTIONAL__
        name: A
        type: edge_list<directedness::directed, T> &
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
    signature_with_names: "template <typename T>\nvoid mm_fill(std::istream & inputStream, edge_list<directedness::directed, T> & A, size_t nNonzeros, bool file_symmetry, bool pattern)"
  "template <typename T>\nvoid mm_fill(std::istream &, edge_list<directedness::undirected, T> &, size_t, bool, bool)":
    arguments:
      - description: __OPTIONAL__
        name: inputStream
        type: std::istream &
      - description: __OPTIONAL__
        name: A
        type: edge_list<directedness::undirected, T> &
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
    signature_with_names: "template <typename T>\nvoid mm_fill(std::istream & inputStream, edge_list<directedness::undirected, T> & A, size_t nNonzeros, bool file_symmetry, bool pattern)"
  void mm_fill(std::istream &, edge_list<directedness::directed> &, size_t, bool, bool):
    arguments:
      - description: __OPTIONAL__
        name: inputStream
        type: std::istream &
      - description: __OPTIONAL__
        name: A
        type: edge_list<directedness::directed> &
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
    signature_with_names: void mm_fill(std::istream & inputStream, edge_list<directedness::directed> & A, size_t nNonzeros, bool file_symmetry, bool pattern)
  void mm_fill(std::istream &, edge_list<directedness::undirected> &, size_t, bool, bool):
    arguments:
      - description: __OPTIONAL__
        name: inputStream
        type: std::istream &
      - description: __OPTIONAL__
        name: A
        type: edge_list<directedness::undirected> &
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
    signature_with_names: void mm_fill(std::istream & inputStream, edge_list<directedness::undirected> & A, size_t nNonzeros, bool file_symmetry, bool pattern)
namespace:
  - nw
  - graph
---
