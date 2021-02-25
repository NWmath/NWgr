---
layout: function
title: tag_invoke
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: access.hpp
overloads:
  "template <class T>\nauto tag_invoke(const nw::graph::degree_tag, T &)":
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: const nw::graph::degree_tag
        unnamed: true
      - description: __OPTIONAL__
        name: n
        type: T &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <class T>\nauto tag_invoke(const nw::graph::degree_tag, T & n)"
  "template <class T>\nauto tag_invoke(const nw::graph::num_vertices_tag, const T &)":
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: const nw::graph::num_vertices_tag
        unnamed: true
      - description: __OPTIONAL__
        name: b
        type: const T &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <class T>\nauto tag_invoke(const nw::graph::num_vertices_tag, const T & b)"
namespace:
  - nw
  - graph
---
