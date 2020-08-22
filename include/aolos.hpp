//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine (c) 2018
//

#ifndef NW_GRAPH_AOLOS_HPP
#define NW_GRAPH_AOLOS_HPP

#include "graph_base.hpp"
#include "edge_list.hpp"
#include <cassert>
#include <forward_list>
#include <tuple>
#include <vector>

namespace nw {
namespace graph {

template <directedness edge_directedness, typename... Attributes>
class edge_list;

template <typename... Attributes>
class array_of_list_of_structs : public std::vector<std::forward_list<std::tuple<Attributes...>>> {

public:
  using base = std::vector<std::forward_list<std::tuple<Attributes...>>>;

  array_of_list_of_structs(size_t N) : base(N) {}

  using inner_iterator      = typename std::forward_list<std::tuple<Attributes...>>::iterator;
  using inner_container_ref = typename std::forward_list<std::tuple<Attributes...>>&;
  using outer_iterator      = typename std::vector<std::forward_list<std::tuple<Attributes...>>>::iterator;

  void open_for_push_back() {}
  void close_for_push_back() {}
  void push_back(size_t i, Attributes... attrs) { base::operator[](i).emplace_front(attrs...); }
};

template <typename... Attributes>
class adj_list : public array_of_list_of_structs<size_t, Attributes...> {
public:
  adj_list(size_t N) : array_of_list_of_structs<size_t, Attributes...>(N) {}
  adj_list(edge_list<directed, Attributes...>& A) : array_of_list_of_structs<size_t, Attributes...>(A.max()[0] + 1) {
    A.fill(*this);
  }
  adj_list(edge_list<undirected, Attributes...>& A) : array_of_list_of_structs<size_t, Attributes...>(A.max()[0] + 1) {
    A.fill(*this);
  }

  using iterator = typename array_of_list_of_structs<size_t, Attributes...>::outer_iterator;

  using attributes_t = std::tuple<Attributes...>;
  static constexpr std::size_t getNAttr() { return sizeof...(Attributes); }
};

}    // namespace graph
}    // namespace nw
#endif    // NW_GRAPH_AOLOS_HPP
