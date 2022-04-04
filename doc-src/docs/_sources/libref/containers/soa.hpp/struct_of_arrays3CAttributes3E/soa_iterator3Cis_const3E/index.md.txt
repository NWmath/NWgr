---
layout: class
title: soa_iterator<is_const>
owner: __MISSING__
brief: __MISSING__
tags:
  - class
defined_in_file: containers/soa.hpp
declaration: "template <bool is_const>\nclass nw::graph::struct_of_arrays::soa_iterator;"
dtor: unspecified
typedefs:
  difference_type:
    definition: std::ptrdiff_t
    description: __MISSING__
  iterator_category:
    definition: std::random_access_iterator_tag
    description: __MISSING__
  pointer:
    definition: arrow_proxy<nw::graph::struct_of_arrays::soa_iterator::reference>
    description: __MISSING__
  reference:
    definition: std::conditional_t<is_const, std::tuple<const Attributes &...>, std::tuple<Attributes &...>>
    description: __MISSING__
  soa_t:
    annotation:
      - private
    definition: std::conditional_t<is_const, const struct_of_arrays<Attributes...>, struct_of_arrays<Attributes...>>
    description: __MISSING__
  value_type:
    definition: std::conditional_t<is_const, std::tuple<const Attributes...>, std::tuple<Attributes...>>
    description: __MISSING__
fields:
  i_:
    annotation:
      - private
    description: __MISSING__
    type: std::size_t
  soa_:
    annotation:
      - private
    description: __MISSING__
    type: nw::graph::struct_of_arrays::soa_iterator::soa_t *
namespace:
  - nw
  - graph
---
