---
layout: method
title: permute
owner: __MISSING__
brief: __MISSING__
tags:
  - method
defined_in_file: soa.hpp
overloads:
  "template <class T, class ExecutionPolicy>\nvoid permute(const std::vector<index_t> &, const std::vector<index_t> &, const std::vector<vertex_id_t> &, T &, ExecutionPolicy &&)":
    arguments:
      - description: __OPTIONAL__
        name: indices
        type: const std::vector<index_t> &
      - description: __OPTIONAL__
        name: new_indices
        type: const std::vector<index_t> &
      - description: __OPTIONAL__
        name: perm
        type: const std::vector<vertex_id_t> &
      - description: __OPTIONAL__
        name: vs
        type: T &
      - description: __OPTIONAL__
        name: ex_policy
        type: ExecutionPolicy &&
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <class T, class ExecutionPolicy>\nvoid permute(const std::vector<index_t> & indices, const std::vector<index_t> & new_indices, const std::vector<vertex_id_t> & perm, T & vs, ExecutionPolicy && ex_policy)"
  void permute(const std::vector<index_t> &, const std::vector<index_t> &, const std::vector<vertex_id_t> &):
    arguments:
      - description: __OPTIONAL__
        name: indices
        type: const std::vector<index_t> &
      - description: __OPTIONAL__
        name: new_indices
        type: const std::vector<index_t> &
      - description: __OPTIONAL__
        name: perm
        type: const std::vector<vertex_id_t> &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: void permute(const std::vector<index_t> & indices, const std::vector<index_t> & new_indices, const std::vector<vertex_id_t> & perm)
---
