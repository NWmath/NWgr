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
//     Xu Tony Liu
//

#ifndef NW_GRAPH_NEIGHBOR_RANGE_HPP
#define NW_GRAPH_NEIGHBOR_RANGE_HPP

#include <queue>
#include <vector>

#include "nwgraph/graph_traits.hpp"
#include "nwgraph/util/util.hpp"


namespace nw {
namespace graph {

template <typename Graph>
class neighbor_range {
public:
  neighbor_range(Graph& g) : the_graph_(g) {}

  class my_iterator {
  private:
    neighbor_range<Graph>&         the_range_;
    typename Graph::outer_iterator first, last;
    typename Graph::outer_iterator G;

  public:
    my_iterator(neighbor_range<Graph>& range) :
      the_range_(range), 
      first(the_range_.the_graph_.begin()), 
      last(the_range_.the_graph_.end()), 
      G(first)
      {}

    my_iterator& operator++() {
      ++first;
      return *this;
    }

    my_iterator operator++(int) const {
      my_iterator tmp(*this);
      ++first;
      return tmp;
    }

    auto operator*() { auto u = first - G; return std::tuple(u, G[u]); }
    auto operator*() const { auto u = first - G; return std::tuple(u, G[u]); }

    class end_sentinel_type {
    public:
      end_sentinel_type() {}
    };

    auto operator==(const end_sentinel_type&) const { return first == last; }
    bool operator!=(const end_sentinel_type&) const { return (first != last); }
  };

  typedef my_iterator iterator;

  auto begin() { return my_iterator(*this); }
  auto end() { return typename my_iterator::end_sentinel_type(); }

private:
  Graph& the_graph_;
};

}    // namespace graph
}    // namespace nw

#endif    // NW_GRAPH_NEIGHBOR_RANGE_HPP
