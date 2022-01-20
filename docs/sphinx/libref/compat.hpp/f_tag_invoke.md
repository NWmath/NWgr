---
layout: function
title: tag_invoke
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: compat.hpp
overloads:
  "template <typename... Attributes>\nauto tag_invoke(const nw::graph::num_edges_tag, const std::list<std::tuple<Attributes...>> &)":
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: const nw::graph::num_edges_tag
        unnamed: true
      - description: __OPTIONAL__
        name: b
        type: const std::list<std::tuple<Attributes...>> &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <typename... Attributes>\nauto tag_invoke(const nw::graph::num_edges_tag, const std::list<std::tuple<Attributes...>> & b)"
  "template <typename... Attributes>\nauto tag_invoke(const nw::graph::num_edges_tag, const std::vector<std::tuple<Attributes...>> &)":
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: const nw::graph::num_edges_tag
        unnamed: true
      - description: __OPTIONAL__
        name: b
        type: const std::vector<std::tuple<Attributes...>> &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <typename... Attributes>\nauto tag_invoke(const nw::graph::num_edges_tag, const std::vector<std::tuple<Attributes...>> & b)"
namespace:
  - nw
  - graph
---
