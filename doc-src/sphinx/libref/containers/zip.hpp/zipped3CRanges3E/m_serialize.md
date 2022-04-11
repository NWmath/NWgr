---
layout: method
title: serialize
owner: __MISSING__
brief: __MISSING__
tags:
  - method
defined_in_file: containers/zip.hpp
overloads:
  "template <class T>\nvoid serialize(std::ostream &, const T &) const":
    arguments:
      - description: __OPTIONAL__
        name: outfile
        type: std::ostream &
      - description: __OPTIONAL__
        name: vs
        type: const T &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <class T>\nvoid serialize(std::ostream & outfile, const T & vs) const"
  void serialize(std::ostream &) const:
    arguments:
      - description: __OPTIONAL__
        name: outfile
        type: std::ostream &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: void serialize(std::ostream & outfile) const
---
