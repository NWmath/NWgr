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

#ifndef NW_GRAPH_NEIGHBOR_RANGE_HPP
#define NW_GRAPH_NEIGHBOR_RANGE_HPP

#include "nwgraph/util/util.hpp"
#include <queue>
#include <vector>

namespace nw {
namespace graph {

template <typename Graph>
class neighbor_range {
public:
  neighbor_range(Graph& g) : the_graph_(g) {}

  class edge_range_iterator {
  private:
    neighbor_range<Graph>&         the_range_;
    typename Graph::outer_iterator first, last;
    typename Graph::outer_iterator G;
    typename Graph::inner_iterator u_begin, u_end;

  public:
    edge_range_iterator(neighbor_range<Graph>& range)
        : the_range_(range), first(the_range_.the_graph_.begin()), last(the_range_.the_graph_.end()), G(first), u_begin((*first).begin()),
          u_end((*first).end()) {}

    edge_range_iterator& operator++() {
      ++u_begin;
      while (u_begin == u_end) {
        if (++first == last) break;
        u_begin = (*first).begin();
        u_end   = (*first).end();
      }

      return *this;
    }

    auto operator*() { return std::tuple<vertex_id_type, vertex_id_type&&>(first - G, std::get<0>(*u_begin)); }

    // return std::tuple<typename Graph::sub_view, typename Graph::sub_view>(*first, G[std::get<0>(*u_begin)]);

    class end_sentinel_type {
    public:
      end_sentinel_type() {}
    };

    auto operator==(const end_sentinel_type&) const { return first == last; }
    bool operator!=(const end_sentinel_type&) const { return (first != last); }
  };

  typedef edge_range_iterator iterator;

  auto begin() { return edge_range_iterator(*this); }
  auto end() { return typename edge_range_iterator::end_sentinel_type(); }

private:
  Graph& the_graph_;
};

}    // namespace graph
}    // namespace nw

#endif    // NW_GRAPH_NEIGHBOR_RANGE_HPP
