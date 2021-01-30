//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine (c) 2018
//

#ifndef NW_GRAPH_VOLOS_HPP
#define NW_GRAPH_VOLOS_HPP

#include <cassert>
#include <forward_list>
#include <tuple>
#include <vector>

#include "edge_list.hpp"
#include "graph_base.hpp"
#include "graph_traits.hpp"

namespace nw {
namespace graph {

template <typename... Attributes>
class vector_of_list_of_structs : public std::vector<std::forward_list<std::tuple<Attributes...>>> {

public:
  using base  = std::vector<std::forward_list<std::tuple<Attributes...>>>;
  using inner = std::forward_list<std::tuple<Attributes...>>;

  using inner_iterator      = typename inner::iterator;
  using inner_container_ref = inner&;
  using outer_iterator      = typename base::iterator;

  vector_of_list_of_structs(size_t N) : base(N) {}
};

template <int idx, std::unsigned_integral vertex_id, typename... Attributes>
class index_adj_list : public unipartite_graph_base, public vector_of_list_of_structs<vertex_id, Attributes...> {
public:
  using vertex_id_type    = vertex_id;
  using base              = vector_of_list_of_structs<vertex_id_type, Attributes...>;
  using graph_base        = unipartite_graph_base;
  using num_vertices_type = std::array<typename base::size_type, 1>;
  using num_edges_type    = base::size_type;

  index_adj_list(size_t N = 0) : base(N) {}

  index_adj_list(edge_list<directedness::directed, Attributes...>& A) : base(A.num_vertices()) { num_edges_ = fill(A, *this); }

  index_adj_list(edge_list<directedness::undirected, Attributes...>& A) : base(A.num_vertices()) { num_edges_ = fill(A, *this); }

  using iterator = typename base::outer_iterator;

  using attributes_t = std::tuple<Attributes...>;
  static constexpr std::size_t getNAttr() { return sizeof...(Attributes); }

  void open_for_push_back() { graph_base::is_open = true; }
  void close_for_push_back() { graph_base::is_open = false; }
  void push_back(size_t i, size_t j, Attributes... attrs) { base::operator[](i).emplace_front(j, attrs...); }

  num_vertices_type num_vertices() const { return {base::size()}; };
  num_edges_type    num_edges() const { return num_edges_; }

private:
  num_edges_type num_edges_;
};

template <int idx, typename... Attributes>
using adj_list = index_adj_list<idx, default_vertex_id_type, Attributes...>;

template <typename... Attributes>
struct graph_traits<std::vector<std::forward_list<std::tuple<Attributes...>>>> {
  using tuple_type = std::tuple<Attributes...>;
  using inner_type = std::forward_list<tuple_type>;
  using outer_type = std::vector<inner_type>;

  using outer_iterator = typename outer_type::iterator;
  using inner_iterator = typename inner_type::iterator;

  using vertex_id_type    = std::tuple_element<0, tuple_type>::type;
  using vertex_size_type  = typename outer_type::size_type;
  using num_vertices_type = std::array<vertex_size_type, 1>;
};

template <typename... Attributes>
struct graph_traits<std::forward_list<std::tuple<Attributes...>>> {
  using tuple_type = std::tuple<Attributes...>;

  using vertex_id_type = std::tuple_element<0, tuple_type>::type;
};

#if 0

template <typename... Attributes>
graph_traits<std::vector<std::forward_list<std::tuple<Attributes...>>>>::num_vertices_type
num_vertices(const typename std::vector<std::forward_list<std::tuple<Attributes...>>>& g) {
  return { g.size() };
}
#endif

}    // namespace graph
}    // namespace nw

#endif    // NW_GRAPH_VOLOS_HPP
