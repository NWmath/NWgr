// 
// This file is part of NW Graph (aka GraphPack) 
// (c) Pacific Northwest National Laboratory 2018-2021 
// (c) University of Washington 2018-2021 
// 
// Licensed under terms of include LICENSE file 
// 
// Authors: 
//     Xu Tony Liu	
//

#include "util/types.hpp"
#include <type_traits>

#ifndef NW_GRAPH_counting_iterator_HPP
#define NW_GRAPH_counting_iterator_HPP

namespace nw {
namespace graph {

template<class T>
class counting_iterator {
private:
    T my_counter_;
public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type        = T;
    using difference_type   = typename std::make_signed<T>::type;
    using reference         = value_type&;
    using pointer           = value_type*;

    counting_iterator() : my_counter_() {}
    explicit counting_iterator(T init) : my_counter_(init) {}

    reference operator*() { return my_counter_; }
    value_type operator[](difference_type index) const { return *(*this + index); }

    difference_type operator-(const counting_iterator& it) const {return my_counter_ - it.my_counter_; }

    counting_iterator& operator+=(difference_type forward) { my_counter_ += forward; return *this; }
    counting_iterator& operator-=(difference_type backward) { my_counter_ -= backward; return *this; }
    counting_iterator& operator++() { return *this += 1; }
    counting_iterator& operator--() { return *this -= 1; }

    counting_iterator operator++(int) {
        counting_iterator it(*this);
        ++(*this);
        return it;
    }

    counting_iterator operator--(int) {
        counting_iterator it(*this);
        --(*this);
        return it;
    }

    counting_iterator operator+(difference_type forward) const { return counting_iterator(my_counter_ + forward); }
    counting_iterator operator-(difference_type backward) const { return counting_iterator(my_counter_ - backward); }
    friend counting_iterator operator+(difference_type forward, const counting_iterator it) { return it + forward; }

    bool operator==(const counting_iterator& it) const { return 0 == (*this - it); }
    bool operator!=(const counting_iterator& it) const { return 0 != (*this - it); }
    bool operator<(const counting_iterator& it) const { return 0 > (*this - it); }
    bool operator>(const counting_iterator& it) const { return 0 < (*this - it); }
    bool operator<=(const counting_iterator& it) const { return 0 >= (*this - it); }
    bool operator>=(const counting_iterator& it) const { return 0 <= (*this - it); }
}; // counting_iterator

/// Vertex_range assumes the vertex id space is from 0 to n - 1. 
/// n is the number of vertices.
template<class Graph>
class vertex_range {
public:
  using difference_type = vertex_id_t;

private:
  counting_iterator<vertex_id_t> begin_;
  counting_iterator<vertex_id_t> end_;

public:
  vertex_range(vertex_id_t max)
      : begin_(counting_iterator<vertex_id_t>(0)), end_(counting_iterator<vertex_id_t>(max)) {}

  vertex_range(const vertex_range&) = default;
  vertex_range(vertex_range&&)      = default;

  vertex_range(vertex_range& rhs, tbb::split)
      : begin_(rhs.begin_), end_(rhs.end_) {}

  counting_iterator<vertex_id_t> begin() const { return begin_; }
  counting_iterator<vertex_id_t> end() const { return end_; }

  difference_type size() const { return end_ - begin_; }
  bool empty() const { return size() == 0; }

  /// Runtime check to see if the range is divisible.
  bool is_divisible() const { return begin_ <= end_; }
};  // vertex_range

}    // namespace graph
}    // namespace nw

#endif