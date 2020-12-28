//
// This file is part of BGL17 (aka NWGraph aka GraphPack aka the Graph Standard Library)
// (c) Pacific Northwest National Laboratory 2019, 2020
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#ifndef NW_GRAPH_SOA_HPP
#define NW_GRAPH_SOA_HPP

#include <cassert>

#include <fstream>
#include <functional>
#include <initializer_list>
#include <iostream>
#include <istream>
#include <iterator>
#include <ostream>
#include <utility>
#include <vector>

#include "util.hpp"

#if defined(CL_SYCL_LANGUAGE_VERSION)
#include <dpstd/algorithm>
#include <dpstd/execution>
#else
#include <algorithm>
#include <execution>
#endif

namespace nw {
namespace graph {

// Bare bones struct of arrays (tuple of vectors)
template <class... Attributes>
struct struct_of_arrays : std::tuple<std::vector<Attributes>...> {
  using storage_type = std::tuple<std::vector<Attributes>...>;
  using base         = std::tuple<std::vector<Attributes>...>;

  template <class... RandomAccessIterators>
  class It {
    std::tuple<RandomAccessIterators...> start;
    std::size_t                          cursor = 0;

    It(const std::tuple<RandomAccessIterators...>& iters, std::size_t init) : start(iters), cursor(init) {}

  public:
    using value_type        = typename std::tuple<typename std::iterator_traits<RandomAccessIterators>::value_type...>;
    using difference_type   = std::ptrdiff_t;
    using reference         = typename std::tuple<typename std::iterator_traits<RandomAccessIterators>::reference...>;
    using pointer           = typename std::tuple<typename std::iterator_traits<RandomAccessIterators>::pointer...>;
    using iterator_category = std::random_access_iterator_tag;

    It()          = default;
    It(const It&) = default;
    It& operator=(const It&) = default;
    It(It&&)                 = default;
    It& operator=(It&&) = default;

    explicit It(const RandomAccessIterators... iters) : start(iters...) {}

    friend void swap(It a, It b) {
      std::apply([&](auto&&... is) { (std::swap(*a.start[is], *b.start[is]), ...); },
                 std::make_index_sequence<sizeof...(Attributes)>());
    }

    It operator++(int) {
      It x(*this);
      ++(*this);
      return x;
    }

    It& operator++() {
      ++cursor;
      return *this;
    }

    It& operator--() {
      --cursor;
      return *this;
    }

    It operator--(int) {
      It x(*this);
      --(*this);
      return x;
    }

    It operator+(std::size_t offset) { return {start, cursor + offset}; }
    It operator+(std::size_t offset) const { return {start, cursor + offset}; }
    It operator-(std::size_t offset) { return {start, cursor - offset}; }
    It operator-(std::size_t offset) const { return {start, cursor - offset}; }

    It& operator+=(std::size_t offset) {
      cursor += offset;
      return *this;
    }

    It& operator-=(std::size_t offset) {
      cursor -= offset;
      return *this;
    }

    difference_type operator-(const It& b) const { return cursor - b.cursor; }

    bool operator==(const It& b) const { return cursor == b.cursor; }

    bool operator!=(const It& b) const { return cursor != b.cursor; }

    reference operator*() {
      return std::apply([&](auto&&... r) { return std::forward_as_tuple(*(std::forward<decltype(r)>(r) + cursor)...); }, start);
    }

    reference operator[](std::size_t i) {
      return std::apply([&](auto&&... r) { return std::forward_as_tuple(std::forward<decltype(r)>(r)[i + cursor]...); }, start);
    }

    reference operator[](std::size_t i) const {
      return std::apply([&](auto&&... r) { return std::forward_as_tuple(std::forward<decltype(r)>(r)[i + cursor]...); }, start);
    }

    bool operator<(const It& x) const { return cursor < x.cursor; }
    bool operator>(const It& x) const { return cursor > x.cursor; }
    bool operator>=(const It& x) const { return cursor >= x.cursor; }
    bool operator<=(const It& x) const { return cursor <= x.cursor; }
  };

  using iterator               = It<typename std::vector<Attributes>::iterator...>;

  using value_type             = iterator::value_type;
  using reference              = iterator::reference;
  using size_type              = std::size_t;
  using difference_type        = iterator::difference_type;
  using pointer                = iterator::pointer;

  using const_iterator         = It<typename std::vector<Attributes>::iterator...>;
  using const_reference        = const_iterator::reference;
  using const_pointer          = const_iterator::pointer;

  using reverse_iterator       = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  struct_of_arrays() = default;
  struct_of_arrays(size_t M) : base(std::vector<Attributes>(M)...) {}

  struct_of_arrays(std::initializer_list<value_type> l) {
    for_each(l.begin(), l.end(), [&](value_type x) { push_back(x); });
  }

  iterator begin() {
    return std::apply([](auto&... vs) { return iterator(vs.begin()...); }, *this);
  }

  iterator begin() const {
    return std::apply([](auto&... vs) { return iterator(vs.begin()...); }, *this);
  }

  iterator end() { return begin() + size(); }

  iterator end() const { return begin() + size(); }

  void push_back(Attributes... attrs) {
    std::apply([&](auto&... vs) { (vs.push_back(attrs), ...); }, *this);
  }

  void push_back(std::tuple<Attributes...> attrs) {
    std::apply([&](Attributes... attr) { push_back(attr...); }, attrs);
  }

  void push_at(std::size_t i, Attributes... attrs) {
    std::apply([&](auto&... vs) { ((vs[i] = attrs), ...); }, *this);
  }

  void push_at(std::size_t i, std::tuple<Attributes...> attrs) {
    std::apply([&](Attributes... attr) { push_at(i, attr...); }, attrs);
  }

  void clear() {
    std::apply([&](auto&... vs) { (vs.clear(), ...); }, *this);
  }

  void resize(size_t N) {
    std::apply([&](auto&&... vs) { (vs.resize(N), ...); }, *this);
  }

  void reserve(size_t N) {
    std::apply([&](auto&&... vs) { (vs.reserve(N), ...); }, *this);
  }

  template <class T>
  void serialize(std::ostream& outfile, const T& vs) const {
    size_t st_size = vs.size();
    size_t el_size = sizeof(vs[0]);
    outfile.write(reinterpret_cast<char*>(&st_size), sizeof(size_t));
    outfile.write(reinterpret_cast<char*>(&el_size), sizeof(size_t));
    outfile.write(reinterpret_cast<const char*>(vs.data()), st_size * el_size);
  }

  template <class T>
  void deserialize(std::istream& infile, T& vs) {
    size_t st_size = -1;
    size_t el_size = -1;
    infile.read(reinterpret_cast<char*>(&st_size), sizeof(size_t));
    infile.read(reinterpret_cast<char*>(&el_size), sizeof(size_t));
    infile.read(reinterpret_cast<char*>(vs.data()), st_size * el_size);
  }

  void serialize(std::ostream& outfile) const {
    size_t st_size = std::get<0>(*this).size();
    size_t el_size = std::tuple_size<storage_type>::value;
    outfile.write(reinterpret_cast<char*>(&st_size), sizeof(size_t));
    outfile.write(reinterpret_cast<char*>(&el_size), sizeof(size_t));
    std::apply([&](auto&... vs) { (serialize(outfile, vs), ...); }, *this);
  }

  void deserialize(std::istream& infile) {
    size_t st_size = -1;
    size_t el_size = -1;
    infile.read(reinterpret_cast<char*>(&st_size), sizeof(size_t));
    infile.read(reinterpret_cast<char*>(&el_size), sizeof(size_t));
    resize(st_size);
    std::apply([&](auto&... vs) { (deserialize(infile, vs), ...); }, *this);
  }

  template <class T, class ExecutionPolicy = std::execution::parallel_unsequenced_policy>
  void permute(const std::vector<index_t>& indices, const std::vector<index_t>& new_indices, const std::vector<vertex_id_t>& perm,
               T& vs, ExecutionPolicy&& ex_policy = {}) {
    T ws(vs.size());
    for (size_t i = 0, e = indices.size() - 1; i < e; ++i) {
      vertex_id_t j = perm[i];
      std::copy(ex_policy, vs.begin() + indices[j], vs.begin() + indices[j + 1], ws.begin() + new_indices[i]);
    }
    std::copy(ex_policy, ws.begin(), ws.end(), vs.begin());
  }

  void permute(const std::vector<index_t>& indices, const std::vector<index_t>& new_indices, const std::vector<vertex_id_t>& perm) {
    std::apply([&](auto&... vs) { (permute(indices, new_indices, perm, vs), ...); }, *this);
  }

  size_t size() const { return std::get<0>(*this).size(); }

  bool operator==(struct_of_arrays& a) { return std::equal(std::execution::par, begin(), end(), a.begin()); }

  bool operator!=(const storage_type& a) { return !operator==(a); }
};

}    // namespace graph
}    // namespace nw

namespace std {
template <class... Attributes>
class tuple_size<nw::graph::struct_of_arrays<Attributes...>> : public std::integral_constant<std::size_t, sizeof...(Attributes)> {};

/// NB: technically we're supposed to be using `iter_swap` here on the
/// struct_of_array iterator type, but I can't figure out how to do this.
template <class... Ts, std::size_t... Is>
void swap(std::tuple<Ts&...>&& x, std::tuple<Ts&...>&& y, std::index_sequence<Is...>) {
  (std::swap(std::get<Is>(x), std::get<Is>(y)), ...);
}

template <class... Ts>
void swap(std::tuple<Ts&...>&& x, std::tuple<Ts&...>&& y) {
  swap(std::move(x), std::move(y), std::make_index_sequence<sizeof...(Ts)>());
}
}    // namespace std

#endif    // NW_GRAPH_SOA_HPP
