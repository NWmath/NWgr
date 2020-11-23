//
// This file is part of BGL17 (aka NWGraph aka GraphPack aka the Graph Standard Library)
// (c) Pacific Northwest National Laboratory 2018
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#ifndef NW_GRAPH_PLAIN_RANGE_HPP
#define NW_GRAPH_PLAIN_RANGE_HPP

#include "util/util.hpp"
#include "util/print_types.hpp"
#include "util/types.hpp"
#include <cassert>
#include <queue>
#include <tuple>
#include <vector>

#if defined(CL_SYCL_LANGUAGE_VERSION)
#include <dpstd/execution>
#include <dpstd/iterators.h>
namespace nw {
namespace graph {
template <class T>
using counting_iterator = dpstd::counting_iterator<T>;
}
}
#else
#include <execution>
#include <tbb/iterators.h>
namespace nw {
namespace graph {
template <class T>
using counting_iterator = tbb::counting_iterator<T>;
}
}
#endif

namespace nw {
namespace graph {


template <typename Graph>
class plain_range {
public:
  plain_range(Graph& g) : the_graph_(g) {}

  class iterator {
    typename Graph::iterator base_;
    typename Graph::iterator first_;

  public:
    iterator(typename Graph::iterator base, typename Graph::iterator first) : base_(base), first_(first) {}

    iterator& operator++() {
      ++first_;
      return *this;
    }

    auto operator*() { return std::tuple(first_ - base_); }

    bool operator==(const iterator& b) const { return first_ == b.first_; }
    bool operator!=(const iterator& b) const { return first_ != b.first_; }
  };

  std::size_t size() const { return the_graph_.size(); }

  iterator begin() { return {the_graph_.begin(), the_graph_.begin()}; }
  iterator end() { return {the_graph_.begin(), the_graph_.end()}; }

private:
  Graph& the_graph_;
};

template <typename Graph>
class plain_degree_range {
public:
  plain_degree_range(Graph& g) : the_graph_(g) {}

  class iterator {
    typename Graph::iterator base_;
    typename Graph::iterator first_;
    typename Graph::iterator last_;

  public:
    iterator(typename Graph::iterator base, typename Graph::iterator first, typename Graph::iterator last)
        : base_(base), first_(first), last_(last) {}

    iterator(const iterator& b) = default;

    iterator& operator++() {
      ++first_;
      return *this;
    }

    auto operator*() { return std::tuple(first_ - base_, (*first_).size()); }

    bool operator==(const iterator& b) const { return first_ == b.first_; }
    bool operator!=(const iterator& b) const { return first_ != b.first_; }
  };

  std::size_t size() const { return the_graph_.size(); }

  iterator begin() { return {the_graph_.begin(), the_graph_.begin(), the_graph_.end()}; }
  iterator end() { return {the_graph_.begin(), the_graph_.end(), the_graph_.end()}; }

private:
  Graph& the_graph_;
};

}    // namespace graph
}    // namespace nw

#endif    // NW_GRAPH_PLAIN_RANGE_HPP
