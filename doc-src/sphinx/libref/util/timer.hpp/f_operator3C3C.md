---
layout: function
title: operator<<
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: util/timer.hpp
overloads:
  std::ostream & operator<<(std::ostream &, const nw::util::ms_timer &):
    arguments:
      - description: __OPTIONAL__
        name: os
        type: std::ostream &
      - description: __OPTIONAL__
        name: t
        type: const nw::util::ms_timer &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: std::ostream & operator<<(std::ostream & os, const nw::util::ms_timer & t)
  std::ostream & operator<<(std::ostream &, const nw::util::seconds_timer &):
    arguments:
      - description: __OPTIONAL__
        name: os
        type: std::ostream &
      - description: __OPTIONAL__
        name: t
        type: const nw::util::seconds_timer &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: std::ostream & operator<<(std::ostream & os, const nw::util::seconds_timer & t)
  std::ostream & operator<<(std::ostream &, const nw::util::us_timer &):
    arguments:
      - description: __OPTIONAL__
        name: os
        type: std::ostream &
      - description: __OPTIONAL__
        name: t
        type: const nw::util::us_timer &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: std::ostream & operator<<(std::ostream & os, const nw::util::us_timer & t)
namespace:
  - nw
  - util
---
