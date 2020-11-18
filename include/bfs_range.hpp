//
// This file is part of BGL17 (aka NWGraph aka GraphPack aka the Graph Standard Library)
// (c) Pacific Northwest National Laboratory 2018
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine, Xu Tony Liu
//

#ifndef NW_GRAPH_BFS_RANGE_HPP
#define NW_GRAPH_BFS_RANGE_HPP

#include "util/util.hpp"
#include "util/types.hpp"
#include <cassert>
#include <queue>
#include <vector>

namespace nw {
namespace graph {

enum status { ready = 0, waiting, processed };
// TODO three style of bfs_range: top-down, bottom-up or direction-opitimizing
// NOT fully verified yet

//****************************************************************************
template <typename Graph, typename Queue = std::queue<vertex_id_t>>
class topdown_bfs_range {
  using Colors = std::vector<status>;

  Graph& graph_;     //!< underlying graph
  Colors colors_;    //!< auxiliary color state
  Queue  queue_;     //!< worklist

  /// Initialize the range with a vertex id.
  void init(vertex_id_t u) {
    queue_.push(u);
    colors_[u] = waiting;
  }

  /// Visit a vertex by enumerating its ready neighbors.
  void visit(vertex_id_t v) {
    for (auto&& [u] : graph_[v]) {
      if (colors_[u] == ready) {
        colors_[u] = waiting;
        queue_.push(u);
      }
    }
  }

  /// Process the next vertex by marking the head as processed, popping it, and
  /// visiting the neighbors.
  void process() {
    if (empty()) return;

    auto u     = queue_.front();
    colors_[u] = processed;
    queue_.pop();

    auto v = queue_.front();
    visit(v);
  }

  /// Get the head vertex.
  decltype(auto) head() { return queue_.front(); }

public:
  topdown_bfs_range(Graph& graph, vertex_id_t seed = 0) : graph_(graph), colors_(graph.size()), queue_() {
    init(seed);
    visit(seed);
  }

  topdown_bfs_range(const topdown_bfs_range&) = delete;

  struct end_sentinel_type {};

  class iterator {
  public:
    iterator(topdown_bfs_range& range) : range_(range) {}

    iterator& operator++() {
      range_.process();
      return *this;
    }

    decltype(auto) operator*() { return range_.head(); }

    bool operator==(const end_sentinel_type&) const { return range_.empty(); }
    bool operator!=(const end_sentinel_type&) const { return !range_.empty(); }

  private:
    topdown_bfs_range& range_;
  };

  iterator          begin() { return {*this}; }
  end_sentinel_type end() { return {}; }
  bool              empty() { return queue_.empty(); }
};    // class topdown_bfs_range

template <typename Graph>
class bottomup_bfs_range {
  using Colors = std::vector<status>;

  Graph&      graph_;
  Colors      colors_;
  vertex_id_t v_;
  vertex_id_t parent_v_;
  size_t      n_;    // number of "processed" vertics

  void advance() {
    // mark current v processed (visited)
    colors_[v_] = processed;
    ++n_;
    ++v_;
    v_ %= graph_.size();
    colors_[v_] = waiting;
    parent_v_   = v_;

    for (auto&& [u] : graph_[v_]) {
      if (colors_[u] != processed) {
        parent_v_ = u;    //mark the first non-processed/unvisited neighbor as parent
        break;            //done with v's neighbors
      }
    }    // for

    // if no valid parent_v, meaning we have travese to the leaf, we mark v
    // processed and be done with it
    if (v_ == parent_v_) {
      colors_[v_] = processed;
      ++n_;
    }
  }

  decltype(auto) next() { return std::tuple(v_, parent_v_); }

public:
  bottomup_bfs_range(Graph& graph, vertex_id_t seed = 0) : graph_(graph), colors_(graph.size()), v_(seed), parent_v_(seed) {
    colors_[seed] = waiting;
    parent_v_     = graph_[seed][0];
  }

  bottomup_bfs_range(const bottomup_bfs_range&) = delete;

  struct end_sentinel_type {};

  class iterator {
  public:
    iterator(bottomup_bfs_range& range) : range_(range) {}

    iterator& operator++() {
      range_.advance();
      return *this;
    }

    decltype(auto) operator*() { return range_.next(); }

    bool operator==(const end_sentinel_type&) const { return range_.empty(); }
    bool operator!=(const end_sentinel_type&) const { return !range_.empty(); }

  private:
    bottomup_bfs_range& range_;
  };

  iterator          begin() { return {*this}; }
  end_sentinel_type end() { return {}; }

  bool empty() { return n_ == graph_.size(); }
};    //class bottomup_bfs_range

}    // namespace graph
}    // namespace nw
#endif    // NW_GRAPH_BFS_RANGE_HPP
