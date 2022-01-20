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

  index_adjacency(size_t N = 0) : unipartite_graph_base(N), base(N) {}
  index_adjacency(size_t N, size_t M = 0) : unipartite_graph_base(N), base(N, M) {}
  index_adjacency(std::array<size_t, 1> N, size_t M = 0) : unipartite_graph_base(N), base(N[0], M) {}

  template <class ExecutionPolicy = std::execution::parallel_unsequenced_policy>
  index_adjacency(index_edge_list<vertex_id_type, unipartite_graph_base, directedness::directed, Attributes...>& A,
                  ExecutionPolicy&&                                                                              policy = {})
      : unipartite_graph_base(A.num_vertices()[0]), base(A.num_vertices()[0] + 1) {
    fill<idx>(A, *this, policy);
  }

  template <class ExecutionPolicy = std::execution::parallel_unsequenced_policy>
  index_adjacency(index_edge_list<vertex_id_type, unipartite_graph_base, directedness::undirected, Attributes...>& A,
                  ExecutionPolicy&&                                                                                policy = {})
      : unipartite_graph_base(A.num_vertices()[0]), base(A.num_vertices()[0] + 1) {
    fill<idx>(A, *this, policy);
  }

  template <class ExecutionPolicy = std::execution::parallel_unsequenced_policy>
  index_adjacency(size_t N,
                  index_edge_list<vertex_id_type, unipartite_graph_base,
                                  directedness::directed, Attributes...>& A,
                  ExecutionPolicy&& policy = {})
      : unipartite_graph_base(N), base(N) {
    fill<idx>(A, *this, policy);
  }
  template <class ExecutionPolicy = std::execution::parallel_unsequenced_policy>
  index_adjacency(size_t N,
                  index_edge_list<vertex_id_type, unipartite_graph_base,
                                  directedness::undirected, Attributes...>& A,
                  ExecutionPolicy&& policy = {})
      : unipartite_graph_base(N), base(N) {
    fill<idx>(A, *this, policy);
  }
  // customized move constructor
  index_adjacency(std::vector<vertex_id_t>&& indices,
                  std::vector<vertex_id_t>&& first_to_be,
                  std::vector<Attributes>&&... rest_to_be)
      : unipartite_graph_base(indices.size() - 1), base(std::move(indices), std::move(first_to_be), std::move(rest_to_be)...) {}
  index_adjacency(std::vector<vertex_id_t>&& indices,
                  std::tuple<std::vector<vertex_id_t>,
                             std::vector<Attributes>...>&& to_be_indexed)
      : unipartite_graph_base(indices.size() - 1), base(std::move(indices), std::move(to_be_indexed)) {}
  // customized copy constructor
  index_adjacency(const std::vector<vertex_id_t>& indices,
                  const std::vector<vertex_id_t>& first_to_be,
                  const std::vector<Attributes>&... rest_to_be)
      : unipartite_graph_base(indices.size() - 1), base(indices, first_to_be, rest_to_be...) {}
  index_adjacency(const std::vector<vertex_id_t>& indices,
                  const std::tuple<std::vector<vertex_id_t>,
                                   std::vector<Attributes>...>& to_be_indexed)
      : unipartite_graph_base(indices.size() - 1), base(indices, to_be_indexed) {}

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

template <int idx, std::unsigned_integral index_type, std::unsigned_integral vertex_id, typename... Attributes>
class index_biadjacency : public bipartite_graph_base, public indexed_struct_of_arrays<index_type, vertex_id, Attributes...> {
  using base = indexed_struct_of_arrays<index_type, vertex_id, Attributes...>;

public:
  using index_t           = index_type;
  using vertex_id_type    = vertex_id;
  using num_vertices_type = std::array<vertex_id_type, 1>;
  using num_edges_type    = index_t;

  // The first index_t isn't considered an attribute.
  using attributes_t = std::tuple<Attributes...>;
  static constexpr std::size_t getNAttr() { return sizeof...(Attributes); }

  index_biadjacency(size_t N0 = 0, size_t N1 = 0, size_t M = 0) : bipartite_graph_base(N0, N1), base(N0, M) {}
  index_biadjacency(std::array<size_t, 2> N, size_t M = 0) : bipartite_graph_base(N[idx], N[(idx + 1) % 2]), base(N[idx], M) {}

  template <class ExecutionPolicy = std::execution::parallel_unsequenced_policy>
  index_biadjacency(index_edge_list<vertex_id_type, bipartite_graph_base, directedness::directed, Attributes...>& A,
                  ExecutionPolicy&&                                                                              policy = {})
      : bipartite_graph_base(A.num_vertices()[idx], A.num_vertices()[(idx + 1) % 2]), base(A.num_vertices()[idx] + 1) {
    fill_biadjacency<idx>(A, *this, policy);
  }

  template <class ExecutionPolicy = std::execution::parallel_unsequenced_policy>
  index_biadjacency(index_edge_list<vertex_id_type, bipartite_graph_base, directedness::undirected, Attributes...>& A,
                  ExecutionPolicy&&                                                                                policy = {})
      : bipartite_graph_base(A.num_vertices()[idx], A.num_vertices()[(idx + 1) % 2]), base(A.num_vertices()[idx] + 1) {
    fill_biadjacency<idx>(A, *this, policy);
  }
  //TODO bipartite_graph_base is not fully initialized
  //TODO only N0 is passed in, missing N1
  // customized move constructor
  index_biadjacency(size_t N1, std::vector<vertex_id_t>&& indices,
                  std::vector<vertex_id_t>&& first_to_be,
                  std::vector<Attributes>&&... rest_to_be)
      : bipartite_graph_base(indices.size() - 1, N1), base(std::move(indices), std::move(first_to_be), std::move(rest_to_be)...) {}
  index_biadjacency(size_t N1, std::vector<vertex_id_t>&& indices,
                  std::tuple<std::vector<vertex_id_t>,
                             std::vector<Attributes>...>&& to_be_indexed)
      : bipartite_graph_base(indices.size() - 1, N1), base(std::move(indices), std::move(to_be_indexed)) {}
  // customized copy constructor
  index_biadjacency(size_t N1, const std::vector<vertex_id_t>& indices,
                  const std::vector<vertex_id_t>& first_to_be,
                  const std::vector<Attributes>&... rest_to_be)
      : bipartite_graph_base(indices.size() - 1, N1), base(indices, first_to_be, rest_to_be...) {}
  index_biadjacency(size_t N1, const std::vector<vertex_id_t>& indices,
                  const std::tuple<std::vector<vertex_id_t>,
                                   std::vector<Attributes>...>& to_be_indexed)
      : bipartite_graph_base(indices.size() - 1, N1), base(indices, to_be_indexed) {}

  auto num_vertices() const { return vertex_cardinality; }
  num_edges_type    num_edges() const { return base::to_be_indexed_.size(); };
};

template <int idx, typename... Attributes>
using biadjacency = index_biadjacency<idx, default_index_t, default_vertex_id_type, Attributes...>;

template <int idx, edge_list_graph edge_list_t>
auto make_biadjacency(edge_list_t& el) {
  return biadjacency<idx>(el);
}


template <int idx, edge_list_c edge_list_t, std::unsigned_integral u_integral, class ExecutionPolicy = std::execution::parallel_unsequenced_policy>
auto make_biadjacency(edge_list_t& el, u_integral n0, u_integral n1, directedness edge_directedness = directedness::directed, ExecutionPolicy&& policy = {}) {
  biadjacency<idx> adj(n0, n1);
  fill_biadjacency<idx>(el, adj, policy);
}


//template <int idx, std::unsigned_integral index_type, std::unsigned_integral vertex_id_type, typename... Attributes>
//auto num_vertices(const index_adjacency<idx, index_type, vertex_id_type, Attributes...>& g) {
//  return g.num_vertices();
//}
//index_adjacency num_vertices CPO
template <int idx, std::unsigned_integral index_type, std::unsigned_integral vertex_id_type, typename... Attributes>
auto tag_invoke(const num_vertices_tag, const index_adjacency<idx, index_type, vertex_id_type, Attributes...>& g) {
  return g.num_vertices()[0];
}
//index_adjacency degree CPO
template <int idx, std::unsigned_integral index_type, std::unsigned_integral vertex_id_type, std::unsigned_integral lookup_type, typename... Attributes>
auto tag_invoke(const degree_tag, const index_adjacency<idx, index_type, vertex_id_type, Attributes...>& g, lookup_type i) {
  return g[i].size();
}
//index_adjacency degree CPO
template <int idx, std::unsigned_integral index_type, std::unsigned_integral vertex_id_type, typename... Attributes>
auto tag_invoke(const degree_tag, const index_adjacency<idx, index_type, vertex_id_type, Attributes...>& g,
                const typename index_adjacency<idx, index_type, vertex_id_type, Attributes...>::sub_view& v) {
  return v.size();
}
//index_biadjacency num_vertices CPO
template <int idx, std::unsigned_integral index_type, std::unsigned_integral vertex_id_type, typename... Attributes>
auto tag_invoke(const num_vertices_tag, const index_biadjacency<idx, index_type, vertex_id_type, Attributes...>& g, int jdx = 0) {
  return g.num_vertices()[jdx];
}
//index_biadjacency degree CPO
template <int idx, std::unsigned_integral index_type, std::unsigned_integral vertex_id_type, std::unsigned_integral lookup_type, typename... Attributes>
auto tag_invoke(const degree_tag, const index_biadjacency<idx, index_type, vertex_id_type, Attributes...>& g, lookup_type i) {
  return g[i].size();
}
//index_biadjacency degree CPO
template <int idx, std::unsigned_integral index_type, std::unsigned_integral vertex_id_type, typename... Attributes>
auto tag_invoke(const degree_tag, const index_biadjacency<idx, index_type, vertex_id_type, Attributes...>& g,
                const typename index_biadjacency<idx, index_type, vertex_id_type, Attributes...>::sub_view& v) {
  return v.size();
}
//degree CPO
template <class Iterator>
auto tag_invoke(const degree_tag, const splittable_range_adapter<Iterator>& n) {
  return n.size();
}

}    // namespace graph
}    // namespace nw

#endif    // NW_GRAPH_ADJACENCY_HPP
