//
// This file is part of NWGraph 
// (c) Pacific Northwest National Laboratory 2018-2020
// (c) University of Washington 2018-2020
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#ifndef NW_GRAPH_ADJACENCY_HPP
#define NW_GRAPH_ADJACENCY_HPP


#include "util/defaults.hpp"
#include "graph_base.hpp"
#include "containers/compressed.hpp"
#include "containers/edge_list.hpp"

#include "build.hpp"

#include <concepts>

namespace nw {
namespace graph {


template <std::unsigned_integral index_type, std::unsigned_integral vertex_id_type, typename... Attributes>
class index_compressed : public unipartite_graph_base, public indexed_struct_of_arrays<index_type, vertex_id_type, Attributes...> {
  using base = indexed_struct_of_arrays<index_type, vertex_id_type, Attributes...>;

public:
  using index_t = index_type;
  using vertex_id_t = vertex_id_type;

  index_compressed(size_t N) : unipartite_graph_base(N), base(N) {}

  void close_for_push_back() { base::close_for_push_back(); };

  auto num_edges() { return base::to_be_indexed_.size(); }
};


template <typename... Attributes>
using compressed = index_compressed<default_index_t, default_vertex_id_t, Attributes...>;


template <int idx, std::unsigned_integral index_type, std::unsigned_integral vertex_id_type, typename... Attributes>
class index_adjacency : public index_compressed<index_type, vertex_id_type, Attributes...> {
  using base = index_compressed<index_type, vertex_id_type, Attributes...>;

public:
  using index_t = index_type;
  using vertex_id_t = vertex_id_type;

  // The first index_t isn't considered an attribute.
  using attributes_t = std::tuple<Attributes...>;
  static constexpr std::size_t getNAttr() { return sizeof...(Attributes); }

  index_adjacency(size_t N = 0, size_t M = 0) : indexed_struct_of_arrays<vertex_id_t, Attributes...>(N, M) {}

  template <class ExecutionPolicy = std::execution::parallel_unsequenced_policy>
  index_adjacency(index_edge_list<vertex_id_type, unipartite_graph_base, directed, Attributes...>& A, ExecutionPolicy&& policy = {}) : base(A.num_vertices()[0]) {
    fill(A, *this, policy);
  }

  template <class ExecutionPolicy = std::execution::parallel_unsequenced_policy>
  index_adjacency(index_edge_list<vertex_id_type, unipartite_graph_base, undirected, Attributes...>& A, ExecutionPolicy&& policy = {}) : base(A.num_vertices()[0]) {
    fill(A, *this, policy);
  }
};



template <int idx, typename... Attributes>
using adjacency = index_adjacency<idx, default_index_t, default_vertex_id_t, Attributes...>;  

}
}

#endif // NW_GRAPH_ADJACENCY_HPP
