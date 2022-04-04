---
layout: function
title: par_read_mm
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: mmio.hpp
overloads:
  "template <nw::graph::directedness sym, typename... Attributes>\nedge_list<sym, Attributes...> par_read_mm(const std::string &, bool, size_t)":
    arguments:
      - description: __OPTIONAL__
        name: filename
        type: const std::string &
      - description: __OPTIONAL__
        name: keep_loops
        type: bool
      - description: __OPTIONAL__
        name: threads
        type: size_t
    description: __OPTIONAL__
    return: __OPTIONAL__
    signature_with_names: "template <nw::graph::directedness sym, typename... Attributes>\nedge_list<sym, Attributes...> par_read_mm(const std::string & filename, bool keep_loops, size_t threads)"
namespace:
  - nw
  - graph
---
