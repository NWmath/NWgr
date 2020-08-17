//
// This file is part of Standard Graph Library (SGL)
// (c) Pacific Northwest National Laboratory 2018-2020
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#ifndef EDGE_RANGE_HPP
#define EDGE_RANGE_HPP

#include "util.hpp"
#include "util/print_types.hpp"
#include "util/types.hpp"
#include <tbb/tbb_stddef.h>
#include <tuple>

template <class Graph, std::size_t... Is>
class edge_range {
  static_assert(((Is < Graph::getNAttr()) && ...), "Attribute index out of range");
  static constexpr size_t cutoff_ = 16;

  typename Graph::iterator outer_base_;
  typename Graph::iterator outer_begin_;
  typename Graph::iterator outer_end_;

public:
  edge_range(Graph& g, std::size_t offset, std::index_sequence<Is...> = {})
      : outer_base_(g.begin()),
        outer_begin_(g.begin() + offset),
        outer_end_(g.end()) {
  }

  edge_range(Graph& g, std::index_sequence<Is...> is = {}) : edge_range(g, 0, is) {
  }

  // Split a range.
  edge_range(edge_range& b, tbb::split) : edge_range(b) {
    auto i = (outer_end_ - outer_begin_) / 2;
    outer_begin_ += i;
    b.outer_end_ = b.outer_begin_ + i;
  }

  // Copy constructors and assignment operators are fine.
  edge_range(const edge_range&) = default;
  edge_range& operator=(const edge_range&) = default;

  class iterator {
  public:
    using iterator_category = std::forward_iterator_tag;
    using        value_type = std::tuple<vertex_id_t, vertex_id_t, std::tuple_element_t<Is, typename Graph::attributes_t>...>;
    using   difference_type = std::size_t;
    using         reference = value_type&;
    using           pointer = value_type*;

  private:
    typename Graph::iterator          base_;    //!<
    typename Graph::iterator         first_;    //!<
    typename Graph::iterator          last_;    //!<
    typename Graph::inner_iterator u_begin_ = {}; //!<
    typename Graph::inner_iterator   u_end_ = {}; //!<

    iterator(const iterator& b, unsigned long step) : iterator(b) {
      first_ += step;
    }

    void check() {
      while (u_begin_ == u_end_ && first_ != last_) {    // make sure we start at a valid dereference point
        if (++first_ != last_) {
          u_begin_ = (*first_).begin();
          u_end_   = (*first_).end();
        }
      }
    }

  public:
    iterator(typename Graph::iterator base, typename Graph::iterator begin, typename Graph::iterator end)
        : base_(base), first_(begin), last_(end)
    {
      if (first_ != last_) {
        u_begin_ = (*first_).begin();
        u_end_   = (*first_).end();
        check();
      }
    }

    // Copy and assignment defaults are fine (we need these because there's not
    // a default constructor.
    iterator(const iterator&) = default;
    iterator& operator=(const iterator& b) = default;

    iterator& operator++() {
      ++u_begin_;
      check();
      return *this;
    }

    auto operator*() {
      return std::tuple(first_ - base_,
                        std::get<0>(*u_begin_),
                        std::ref(std::get<Is + 1>(*u_begin_))...);
    }

    auto operator*() const {
      return std::tuple(first_ - base_,
                        std::get<0>(*u_begin_),
                        std::ref(std::get<Is + 1>(*u_begin_))...);
    }

    bool operator==(const iterator& b) const { return first_ == b.first_; }
    bool operator!=(const iterator& b) const { return first_ != b.first_; }
    bool  operator<(const iterator& b) const { return first_  < b.first_; }

    difference_type operator-(const iterator& b)    const { return first_ - b.first_; }
    iterator        operator+(difference_type step) const { return iterator(*this, step); }
  };

  iterator begin()       { return { outer_base_, outer_begin_, outer_end_ }; }
  iterator begin() const { return { outer_base_, outer_begin_, outer_end_ }; }

  iterator end()         { return { outer_base_, outer_end_, outer_end_ }; }
  iterator end()   const { return { outer_base_, outer_end_, outer_end_ }; }

  std::size_t  size() const { return outer_end_ - outer_begin_; }
  bool        empty() const { return begin() == end(); }
  bool is_divisible() const { return size() > cutoff_; }
};

template <std::size_t... Is, class Graph>
static inline edge_range<Graph, Is...> make_edge_range(Graph& g, std::size_t offset) {
  return { g, offset };
}

template <std::size_t... Is, class Graph>
static inline edge_range<Graph, Is...> make_edge_range(Graph& g) {
  return { g };
}

#endif    // EDGE_RANGE_HPP
