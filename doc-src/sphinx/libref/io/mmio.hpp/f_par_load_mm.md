---
layout: function
title: par_load_mm
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: mmio.hpp
overloads:
  std::tuple<size_t, std::array<vertex_id_t, 2>, std::array<vertex_id_t, 2>> par_load_mm(mmio::MatrixMarketFile &, std::vector<std::vector<std::tuple<size_t, size_t>>> &, std::vector<std::vector<std::tuple<size_t, size_t>>> &, bool, size_t):
    arguments:
      - description: __OPTIONAL__
        name: mmio
        type: mmio::MatrixMarketFile &
      - description: __OPTIONAL__
        name: sub_lists
        type: std::vector<std::vector<std::tuple<size_t, size_t>>> &
      - description: __OPTIONAL__
        name: sub_loops
        type: std::vector<std::vector<std::tuple<size_t, size_t>>> &
      - description: __OPTIONAL__
        name: keep_loops
        type: bool
      - description: __OPTIONAL__
        name: threads
        type: size_t
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: std::tuple<size_t, std::array<vertex_id_t, 2>, std::array<vertex_id_t, 2>> par_load_mm(mmio::MatrixMarketFile & mmio, std::vector<std::vector<std::tuple<size_t, size_t>>> & sub_lists, std::vector<std::vector<std::tuple<size_t, size_t>>> & sub_loops, bool keep_loops, size_t threads)
  "template <typename T>\nstd::tuple<size_t, std::array<vertex_id_t, 2>, std::array<vertex_id_t, 2>> par_load_mm(mmio::MatrixMarketFile &, std::vector<std::vector<std::tuple<size_t, size_t, T>>> &, std::vector<std::vector<std::tuple<size_t, size_t, T>>> &, bool, size_t)":
    arguments:
      - description: __OPTIONAL__
        name: mmio
        type: mmio::MatrixMarketFile &
      - description: __OPTIONAL__
        name: sub_lists
        type: std::vector<std::vector<std::tuple<size_t, size_t, T>>> &
      - description: __OPTIONAL__
        name: sub_loops
        type: std::vector<std::vector<std::tuple<size_t, size_t, T>>> &
      - description: __OPTIONAL__
        name: keep_loops
        type: bool
      - description: __OPTIONAL__
        name: threads
        type: size_t
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <typename T>\nstd::tuple<size_t, std::array<vertex_id_t, 2>, std::array<vertex_id_t, 2>> par_load_mm(mmio::MatrixMarketFile & mmio, std::vector<std::vector<std::tuple<size_t, size_t, T>>> & sub_lists, std::vector<std::vector<std::tuple<size_t, size_t, T>>> & sub_loops, bool keep_loops, size_t threads)"
namespace:
  - nw
  - graph
---
