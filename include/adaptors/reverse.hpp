//
// This file is part of BGL17 (aka NWGraph aka GraphPack aka the Graph Standard Library)
// (c) Pacific Northwest National Laboratory 2018
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Kevin Deweese
//

#ifndef NW_GRAPH_REVERSE_HPP
#define NW_GRAPH_REVERSE_HPP

#include "util/types.hpp"
#include "util/util.hpp"
#include <vector>

namespace nw {
namespace graph {

template <typename path_edge>
class reverse_path {
public:
  reverse_path(std::vector<path_edge>& path, vertex_id_t start, vertex_id_t stop) : path_(path), start_(start), stop_(stop) {}

  std::vector<path_edge> path_;
  double                 update_;
  vertex_id_t            start_, stop_;
  bool                   done = false;

  bool found(vertex_id_t current) { return current == stop_; }

  class reverse_iterator {
  private:
    reverse_path& rev_;
    vertex_id_t   curr_;

  public:
    reverse_iterator(reverse_path& rev) : rev_(rev), curr_(rev.start_) {}

    reverse_iterator& operator++() {
      curr_ = rev_.path_[curr_].predecessor;
      if (rev_.found(curr_)) {
        rev_.done = true;
      }

      return *this;
    }

    auto operator*() { return std::tuple<vertex_id_t>(curr_); }

    class end_sentinel_type {
    public:
      end_sentinel_type() {}
    };

    auto operator==(const end_sentinel_type&) const { return rev_.done; }
    bool operator!=(const end_sentinel_type&) const { return !rev_.done; }
  };

  auto begin() { return reverse_iterator(*this); }
  auto end() { return typename reverse_iterator::end_sentinel_type(); }
};

}    // namespace graph
}    // namespace nw
#endif    // NW_GRAPH_REVERSE_HPP
