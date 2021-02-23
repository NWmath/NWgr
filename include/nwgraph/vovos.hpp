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

#ifndef NW_GRAPH_VOVOS_HPP
#define NW_GRAPH_VOVOS_HPP

#include <algorithm>
#include <cassert>
#include <concepts>
#include <tuple>
#include <vector>

#include "edge_list.hpp"
#include "nwgraph/graph_base.hpp"

namespace nw {
namespace graph {

template <typename... Attributes>
class vector_of_vector_of_structs : public std::vector<std::vector<std::tuple<Attributes...>>> {

public:
  using base = std::vector<std::vector<std::tuple<Attributes...>>>;

  vector_of_vector_of_structs(size_t N) : base(N) {}

  using inner_iterator       = typename std::vector<std::tuple<Attributes...>>::iterator;
  using const_inner_iterator = typename std::vector<std::tuple<Attributes...>>::const_iterator;
  using inner_container_ref  = typename std::vector<std::tuple<Attributes...>>&;
  using outer_iterator       = typename std::vector<std::vector<std::tuple<Attributes...>>>::iterator;
  using const_outer_iterator = typename std::vector<std::vector<std::tuple<Attributes...>>>::const_iterator;

  void open_for_push_back() {}
  void close_for_push_back() {}
  void push_back(size_t i, Attributes... attrs) { base::operator[](i).emplace_back(attrs...); }

  auto size() const { return base::size(); }
};

template <int idx, std::unsigned_integral vertex_id, typename... Attributes>
class index_vov : public unipartite_graph_base, public vector_of_vector_of_structs<vertex_id, Attributes...> {
  using base = vector_of_vector_of_structs<vertex_id, Attributes...>;

public:
  using vertex_id_type    = vertex_id;
  using graph_base        = unipartite_graph_base;
  using num_vertices_type = std::array<typename base::size_type, 1>;
  using num_edges_type    = typename base::size_type;

  using attributes_t = std::tuple<Attributes...>;

  static constexpr std::size_t getNAttr() { return sizeof...(Attributes); }

  index_vov(size_t N) : base(N) {}

  index_vov(edge_list<directedness::directed, Attributes...>& A) : base(num_vertices(A)) { num_edges_ = fill_adj_list(A, *this); }

  index_vov(edge_list<directedness::undirected, Attributes...>& A) : base(num_vertices(A)) { num_edges_ = fill_adj_list(A, *this); }

private:
  num_edges_type num_edges_;
};

template <int idx, typename... Attributes>
using vov = index_vov<idx, default_vertex_id_type, Attributes...>;

template <typename... Attributes>
struct graph_traits<std::vector<std::vector<std::tuple<Attributes...>>>> {
  using tuple_type = std::tuple<Attributes...>;
  using inner_type = std::vector<tuple_type>;
  using outer_type = std::vector<inner_type>;

  using outer_iterator = typename outer_type::iterator;
  using inner_iterator = typename inner_type::iterator;

  using const_outer_iterator = typename outer_type::const_iterator;
  using const_inner_iterator = typename inner_type::const_iterator;

  using vertex_id_type    = typename std::tuple_element<0, tuple_type>::type;
  using vertex_size_type  = typename outer_type::size_type;
  using num_vertices_type = std::array<vertex_size_type, 1>;
};

template <typename... Attributes>
struct graph_traits<std::vector<std::tuple<Attributes...>>> {
  using tuple_type = std::tuple<Attributes...>;

  using vertex_id_type = typename std::tuple_element<0, tuple_type>::type;
};

}    // namespace graph
}    // namespace nw
#endif    // NW_GRAPH_VOVOS_HPP