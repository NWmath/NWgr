---
layout: function
title: read_mm
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: io/mmio.hpp
overloads:
  "template <nw::graph::directedness sym, typename... Attributes>\nedge_list<sym, Attributes...> read_mm(const std::string &)":
    arguments:
      - description: __OPTIONAL__
        name: filename
        type: const std::string &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <nw::graph::directedness sym, typename... Attributes>\nedge_list<sym, Attributes...> read_mm(const std::string & filename)"
  "template <nw::graph::directedness sym, typename... Attributes>\nedge_list<sym, Attributes...> read_mm(std::istream &)":
    arguments:
      - description: __OPTIONAL__
        name: inputStream
        type: std::istream &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <nw::graph::directedness sym, typename... Attributes>\nedge_list<sym, Attributes...> read_mm(std::istream & inputStream)"
namespace:
  - nw
  - graph
---
