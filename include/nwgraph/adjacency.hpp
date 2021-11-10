// 
// This file is part of NW Graph (aka GraphPack) 
// (c) Pacific Northwest National Laboratory 2018-2021 
// (c) University of Washington 2018-2021 
// 
// Licensed under terms of include LICENSE file 
// 
// Authors: 
//     Andrew Lumsdaine	
//

#ifndef NW_GRAPH_ADJACENCY_HPP
#define NW_GRAPH_ADJACENCY_HPP

#include "nwgraph/containers/compressed.hpp"
#include "nwgraph/edge_list.hpp"
#include "nwgraph/graph_base.hpp"
#include "nwgraph/graph_concepts.hpp"
#include "nwgraph/util/defaults.hpp"

#include "nwgraph/build.hpp"

#include <concepts>

#include "nwgraph/graph_concepts.hpp"


namespace nw {
namespace graph {

#if 0
template <std::unsigned_integral index_type, std::unsigned_integral vertex_id_type, typename... Attributes>
class index_compressed : public unipartite_graph_base, public indexed_struct_of_arrays<index_type, vertex_id_type, Attributes...> {
  using base = indexed_struct_of_arrays<index_type, vertex_id_type, Attributes...>;

public:
  using index_t = index_type;
  using vertex_id_type = vertex_id_type;

  index_compressed(size_t N) : unipartite_graph_base(N), base(N) {}

  void close_for_push_back() { base::close_for_push_back(); };

  auto num_edges() { return base::to_be_indexed_.size(); }
};


template <typename... Attributes>
using compressed = index_compressed<default_index_t, default_vertex_id_type, Attributes...>;
#endif

template <int idx, std::unsigned_integral index_type, std::unsigned_integral vertex_id, typename... Attributes>
class index_adjacency : public unipartite_graph_base, public indexed_struct_of_arrays<index_type, vertex_id, Attributes...> {
  using base = indexed_struct_of_arrays<index_type, vertex_id, Attributes...>;

public:
  using index_t           = index_type;
  using vertex_id_type    = vertex_id;
  using num_vertices_type = std::array<vertex_id_type, 1>;
  using num_edges_type    = index_t;

  // The first index_t isn't considered an attribute.
  using attributes_t = std::tuple<Attributes...>;
  static constexpr std::size_t getNAttr() { return sizeof...(Attributes); }

  index_adjacency(size_t N = 0, size_t M = 0) : base(N, M) {}
  index_adjacency(std::array<size_t, 1> N, size_t M = 0) : base(N[0], M) {}

  template <class ExecutionPolicy = std::execution::parallel_unsequenced_policy>
  index_adjacency(index_edge_list<vertex_id_type, unipartite_graph_base, directedness::directed, Attributes...>& A,
                  ExecutionPolicy&&                                                                              policy = {})
      : base(A.num_vertices()[0]) {
    fill<idx>(A, *this, policy);
  }

  template <class ExecutionPolicy = std::execution::parallel_unsequenced_policy>
  index_adjacency(index_edge_list<vertex_id_type, unipartite_graph_base, directedness::undirected, Attributes...>& A,
                  ExecutionPolicy&&                                                                                policy = {})
      : base(A.num_vertices()[0]) {
    fill<idx>(A, *this, policy);
  }

  num_vertices_type num_vertices() const { return {base::size()}; };
  num_edges_type    num_edges() const { return base::to_be_indexed_.size(); };
};

template <int idx, typename... Attributes>
using adjacency = index_adjacency<idx, default_index_t, default_vertex_id_type, Attributes...>;

template <int idx, edge_list_graph edge_list_t>
auto make_adjacency(edge_list_t& el) {
  return adjacency<idx>(el);
}


template <int idx, edge_list_c edge_list_t, std::unsigned_integral u_integral, class ExecutionPolicy = std::execution::parallel_unsequenced_policy>
auto make_adjacency(edge_list_t& el, u_integral n, directedness edge_directedness = directedness::directed, ExecutionPolicy&& policy = {}) {
  adjacency<idx> adj(n);
  fill<idx>(el, adj, edge_directedness, policy);
}


//template <int idx, std::unsigned_integral index_type, std::unsigned_integral vertex_id_type, typename... Attributes>
//auto num_vertices(const index_adjacency<idx, index_type, vertex_id_type, Attributes...>& g) {
//  return g.num_vertices();
//}
//num_vertices CPO
template <int idx, std::unsigned_integral index_type, std::unsigned_integral vertex_id_type, typename... Attributes>
auto tag_invoke(const num_vertices_tag, const index_adjacency<idx, index_type, vertex_id_type, Attributes...>& g) {
  return g.num_vertices()[0];
}
//degree CPO
template <int idx, std::unsigned_integral index_type, std::unsigned_integral vertex_id_type, std::unsigned_integral lookup_type, typename... Attributes>
auto tag_invoke(const degree_tag, const index_adjacency<idx, index_type, vertex_id_type, Attributes...>& g, lookup_type i) {
  return g[i].size();
}
//degree CPO
template <int idx, std::unsigned_integral index_type, std::unsigned_integral vertex_id_type, typename... Attributes>
auto tag_invoke(const degree_tag, const index_adjacency<idx, index_type, vertex_id_type, Attributes...>& g,
                const typename index_adjacency<idx, index_type, vertex_id_type, Attributes...>::sub_view& v) {
  return v.size();
}
//degree CPO
template <class Iterator>
auto tag_invoke(const degree_tag, const splittable_range_adapter<Iterator>& n) {
  return n.size();
}


  //template <int idx, std::unsigned_integral index_type, std::unsigned_integral vertex_id_type, typename... Attributes>
  //auto tag_invoke(const degree_tag, typename index_adjacency<idx, index_type, vertex_id_type, Attributes...>::sub_view& n) {
  //  return n.size();
  //}



  //template <int idx, std::unsigned_integral index_type, std::unsigned_integral vertex_id_type, typename... Attributes>
  //auto tag_invoke(const degree_tag, typename index_adjacency<idx, index_type, vertex_id_type, Attributes...>::sub_view& n) {
  //  return n.size();
  //}


}    // namespace graph
}    // namespace nw

#endif    // NW_GRAPH_ADJACENCY_HPP
