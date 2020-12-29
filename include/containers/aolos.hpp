//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine (c) 2018
//

#ifndef NW_GRAPH_AOLOS_HPP
#define NW_GRAPH_AOLOS_HPP

#include "edge_list.hpp"
#include "graph_base.hpp"
#include <cassert>
#include <forward_list>
#include <tuple>
#include <vector>

namespace nw {
namespace graph {

template <directedness edge_directedness, typename... Attributes>
class edge_list;

template <typename... Attributes>
class array_of_list_of_structs {

public:
  array_of_list_of_structs(size_t N) : storage_(N) {}

  using inner_iterator      = typename std::forward_list<std::tuple<Attributes...>>::iterator;
  using inner_container_ref = typename std::forward_list<std::tuple<Attributes...>>&;
  using outer_iterator      = typename std::vector<std::forward_list<std::tuple<Attributes...>>>::iterator;

  auto   begin() { return storage_.begin(); }
  auto   end() { return storage_.end(); }
  auto   begin() const { return storage_.begin(); }
  auto   end() const { return storage_.end(); }
  size_t size() { return storage_.end() - storage_.begin(); }

  void open_for_push_back() {}
  void close_for_push_back() {}
  void push_back(size_t i, Attributes... attrs) { storage_[i].emplace_front(attrs...); }

private:
  std::vector<std::forward_list<std::tuple<Attributes...>>> storage_;
};

template <typename... Attributes>
class adj_list : public array_of_list_of_structs<size_t, Attributes...> {
public:
  adj_list(size_t N) : array_of_list_of_structs<size_t, Attributes...>(N) {}
  adj_list(edge_list<directed, Attributes...>& A)
      : array_of_list_of_structs<size_t, Attributes...>(std::max(A.max()[0], A.max()[1]) + 1) {
    A.fill(*this);
  }
  adj_list(edge_list<undirected, Attributes...>& A)
      : array_of_list_of_structs<size_t, Attributes...>(std::max(A.max()[0], A.max()[1]) + 1) {
    A.fill(*this);
  }

  using iterator = typename array_of_list_of_structs<size_t, Attributes...>::outer_iterator;

  using attributes_t = std::tuple<Attributes...>;
  static constexpr std::size_t getNAttr() { return sizeof...(Attributes); }
};

}    // namespace graph
}    // namespace nw
#endif    // NW_GRAPH_AOLOS_HPP
