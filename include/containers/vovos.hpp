//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine (c) 2018
//

#ifndef NW_GRAPH_VOVOS_HPP
#define NW_GRAPH_VOVOS_HPP

#include <algorithm>
#include <cassert>
#include <forward_list>
#include <tuple>
#include <vector>


#include "edge_list.hpp"
#include "graph_base.hpp"

namespace nw {
namespace graph {

template <typename... Attributes>
class vector_of_vector_of_structs : public std::vector<std::forward_list<std::tuple<Attributes...>>> {

public:
  using base = std::vector<std::forward_list<std::tuple<Attributes...>>>;

  vector_of_vector_of_structs(size_t N) : base(N) {}

  using inner_iterator      = typename std::forward_list<std::tuple<Attributes...>>::iterator;
  using inner_container_ref = typename std::forward_list<std::tuple<Attributes...>>&;
  using outer_iterator      = typename std::vector<std::forward_list<std::tuple<Attributes...>>>::iterator;

  void open_for_push_back() {}
  void close_for_push_back() {}
  void push_back(size_t i, Attributes... attrs) { base::operator[](i).emplace_front(attrs...); }

  auto size() const { return base::size(); }
};

template <typename... Attributes>
class vov : public vector_of_vector_of_structs<size_t, Attributes...> {
  using base = vector_of_vector_of_structs<size_t, Attributes...>;

public:
  using attributes_t = std::tuple<Attributes...>;
  static constexpr std::size_t getNAttr() { return sizeof...(Attributes); }

  vov(size_t N) : vector_of_vector_of_structs<size_t, Attributes...>(N) {}

  template <directedness dir>
  vov(/* const */ edge_list<dir, Attributes...>& A) : vector_of_vector_of_structs<size_t, Attributes...>(0) {
    dynamic_cast<base&>(*this).open_for_push_back();

    if constexpr (dir == directed) {
      (*this).resize(A.max()[0] + 1);
      std::for_each(A.begin(), A.end(), [&](auto&& elt) {
        std::apply([&](vertex_id_t i, vertex_id_t j, Attributes... attrs) { (*this).push_back(i, j, attrs...); }, elt);
      });
    } else if constexpr (dir == undirected) {
      (*this).resize(2 * (A.max()[0] + 1));
      std::for_each(A.begin(), A.end(), [&](auto&& elt) {
        std::apply([&](vertex_id_t i, vertex_id_t j, Attributes... attrs) { (*this).push_back(i, j, attrs...); }, elt);
        std::apply([&](vertex_id_t i, vertex_id_t j, Attributes... attrs) { (*this).push_back(j, i, attrs...); }, elt);
      });
    } else {
      ;
    }

    dynamic_cast<base&>(*this).close_for_push_back();
  }
};

}    // namespace graph
}    // namespace nw
#endif    // NW_GRAPH_VOVOS_HPP
