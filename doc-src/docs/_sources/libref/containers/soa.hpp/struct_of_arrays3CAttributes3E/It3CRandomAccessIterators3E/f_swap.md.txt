---
layout: function
title: swap
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: soa.hpp
overloads:
  "template <class... Ts, std::size_t... Is>\nvoid swap(std::tuple<Ts &...> &&, std::tuple<Ts &...> &&, std::index_sequence<Is...>)":
    arguments:
      - description: __OPTIONAL__
        name: x
        type: std::tuple<Ts &...> &&
      - description: __OPTIONAL__
        name: y
        type: std::tuple<Ts &...> &&
      - description: __OPTIONAL__
        name: unnamed-2
        type: std::index_sequence<Is...>
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <class... Ts, std::size_t... Is>\nvoid swap(std::tuple<Ts &...> && x, std::tuple<Ts &...> && y, std::index_sequence<Is...>)"
  "template <class... Ts>\nvoid swap(std::tuple<Ts &...> &&, std::tuple<Ts &...> &&)":
    arguments:
      - description: __OPTIONAL__
        name: x
        type: std::tuple<Ts &...> &&
      - description: __OPTIONAL__
        name: y
        type: std::tuple<Ts &...> &&
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <class... Ts>\nvoid swap(std::tuple<Ts &...> && x, std::tuple<Ts &...> && y)"
  void swap(It<RandomAccessIterators...>, It<RandomAccessIterators...>):
    arguments:
      - description: __OPTIONAL__
        name: a
        type: It<RandomAccessIterators...>
      - description: __OPTIONAL__
        name: b
        type: It<RandomAccessIterators...>
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: void swap(It<RandomAccessIterators...> a, It<RandomAccessIterators...> b)
namespace:
  - nw
  - graph
---
