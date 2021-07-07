// 
// This file is part of NW Graph (aka GraphPack) 
// (c) Pacific Northwest National Laboratory 2018-2021 
// (c) University of Washington 2018-2021 
// 
// Licensed under terms of include LICENSE file 
// 
// Authors: 
//     Andrew Lumsdaine	
//     Kevin Deweese	
//
#ifndef NW_GRAPH_CYCLIC_RANGE_ADAPTER_HPP
#define NW_GRAPH_CYCLIC_RANGE_ADAPTER_HPP

#include "nwgraph/util/util.hpp"
#include <iterator>
#if NW_GRAPH_NEED_TBB
#include <tbb/tbb.h>
#include <tbb/tbb_stddef.h>
#endif

namespace nw {
namespace graph {
/// stride rather than in terms of the number of elements. A _cutoff_ of `1`
/// implies that the range can't be split, while a `_cutoff_` of `n` means that
/// the range can be split into up to `n` cycles.
///
/// @tparam    Iterator The type of the underlying range iterator (must be at
///                     least random access).
template <class Iterator>
class cyclic_range_adapter {
  static_assert(std::is_base_of_v<std::random_access_iterator_tag, typename std::iterator_traits<Iterator>::iterator_category>);

public:
  using difference_type = typename std::iterator_traits<Iterator>::difference_type;

private:
  Iterator        begin_;
  Iterator        end_;
  difference_type cutoff_;
  difference_type cycle_  = 0;
  difference_type stride_ = 1;

public:
  template <class Range, class Cutoff>
  cyclic_range_adapter(Range&& range, Cutoff cutoff)
      : begin_(range.begin()), end_(range.end()), cutoff_(nw::graph::pow2(nw::graph::ceil_log2(cutoff))) {}

  cyclic_range_adapter(const cyclic_range_adapter&) = default;
  cyclic_range_adapter(cyclic_range_adapter&&)      = default;

#if NW_GRAPH_NEED_TBB
  cyclic_range_adapter(cyclic_range_adapter& rhs, tbb::split)
      : begin_(rhs.begin_), end_(rhs.end_), cutoff_(rhs.cutoff_), cycle_(rhs.cycle_ + rhs.stride_), stride_(rhs.stride_ *= 2) {}
#endif

  struct iterator {
    using traits            = std::iterator_traits<Iterator>;
    using pointer           = typename traits::pointer;
    using value_type        = typename traits::value_type;
    using reference         = typename traits::reference;
    using difference_type   = typename traits::difference_type;
    using iterator_category = typename traits::iterator_category;

    Iterator        i_;
    difference_type stride_;

    decltype(auto) operator*() { return *i_; }

    iterator& operator++() {
      i_ += stride_;
      return *this;
    }

    iterator operator++(int) {
        iterator x(*this);
        ++(*this);
        return x;
    }

    iterator& operator--() {
        i_ -= stride_;
        return *this;
    }

    iterator operator--(int) {
        iterator x(*this);
        --(*this);
        return x;
    }

    iterator& operator+=(std::size_t offset) {
        i_ += stride_ * offset;
        return *this;
    }

    iterator& operator-=(std::size_t offset) {
        i_ -= stride_ * offset;
        return *this;
    }

    difference_type operator-(iterator const& rhs) const {
        return (i_ - rhs.i_) / stride_;
    }

    bool operator==(const iterator& rhs) const {
        return i_ == rhs.i_;
    }
    bool operator!=(const iterator& rhs) const { return i_ != rhs.i_; }
  };

  /// Return an iterator that points to the start of the cycle.
  iterator begin() { return {begin_ + cycle_, stride_}; }

  /// Return an iterator that points to the end of the cycle.
  ///
  /// The end of the cycle is the first iterator in the cycle that is greater
  /// than or equal to the end_ iterator in the underlying range. End iterators
  /// for different cycles will be different even if the underlying range and
  /// strides match, so tests should not be performed across cycles.
  iterator end() {
    difference_type n = end_ - begin_ - cycle_;     // shifted span for cycle
    difference_type r = n % stride_;                // remainder in last stride
    difference_type e = (stride_ - r) % stride_;    // amount past `end_` we'll go
    return {end_ + e, stride_};
  }

  difference_type size() const {
    // figure out the number of valid elements in our cycle, which depends on
    // the total number of elements, where our cycle starts, and the stride
    difference_type n = end_ - begin_;
    return n / stride_ + ((cycle_ < n % stride_) ? 1 : 0);
  }

  bool empty() const { return size() == 0; }

  /// Runtime check to see if the range is divisible.
  ///
  /// The range can be subdivided if its stride can be increased relative to the
  /// cutoff.
  bool is_divisible() const { return stride_ <= cutoff_; }
};

template <class Range, class Cutoff>
cyclic_range_adapter(Range&& range, Cutoff) -> cyclic_range_adapter<decltype(range.begin())>;

template <class Range, class Cutoff>
constexpr decltype(auto) cyclic(Range&& range, Cutoff cutoff) {
  return cyclic_range_adapter{std::forward<Range>(range), cutoff};
}
}    // namespace graph
}    // namespace nw
#endif    // NW_GRAPH_CYCLIC_RANGE_ADAPTER_HPP
