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

#ifndef NW_GRAPH_GRAPH_CONCEPTS_HPP
#define NW_GRAPH_GRAPH_CONCEPTS_HPP

#include <array>
#include <concepts>
#include <iterator>
#include <ranges>
#include <tuple>

#include "nwgraph/graph_traits.hpp"


namespace nw::graph {

  /**
   * Forward-declare our CPO tags
   */
DECL_TAG_INVOKE(num_vertices);
DECL_TAG_INVOKE(num_edges);
DECL_TAG_INVOKE(degree);
DECL_TAG_INVOKE(source);
DECL_TAG_INVOKE(target);

template <typename G>
concept graph = std::semiregular<G> && requires(G g) {
  typename vertex_id_t<G>;
  { num_vertices(g) } -> std::convertible_to<std::ranges::range_difference_t<G>>;
};

template <typename G>
using inner_range_t = std::ranges::range_value_t<G>;

template <typename G>
using inner_value_t = std::ranges::range_value_t<inner_range_t<G>>;

template <typename G>
concept adjacency_list_graph = graph<G>
  && std::ranges::random_access_range<G>
  && std::ranges::forward_range<inner_range_t<G>>
  && std::convertible_to<vertex_id_t<G>,std::ranges::range_difference_t<G>>
  && requires(G g, vertex_id_t<G> u, inner_value_t<G> e) {
  { g[u] } -> std::convertible_to<inner_range_t<G>>;
  { target(g, e) } -> std::convertible_to<vertex_id_t<G>>;
};

template <typename G>
concept degree_enumerable_graph = adjacency_list_graph<G>
  && requires (G g, vertex_id_t<G> u) {
  { degree(g[u]) } -> std::convertible_to<std::ranges::range_difference_t<G>>;
};

template <typename G>
concept edge_list_graph = graph<G>
  && requires (G g, std::ranges::range_value_t<G> e) {
  { source(g, e) } -> std::convertible_to<vertex_id_t<G>>;
  { target(g, e) } -> std::convertible_to<vertex_id_t<G>>;
};


// Some default traits for common classes of graphs

template <template <class> class Outer, template <class> class Inner, std::integral Index, typename... Attributes>
requires std::ranges::random_access_range<Outer<Inner<std::tuple<Index, Attributes...>>>> &&
      std::ranges::forward_range<Inner<std::tuple<Index, Attributes...>>>
struct graph_traits<Outer<Inner<std::tuple<Index, Attributes...>>>> {
  using vertex_id_type = Index;
};

template <template <class> class Outer, template <class> class Inner, std::integral Index>
requires std::ranges::random_access_range<Outer<Inner<Index>>> && std::ranges::forward_range<Inner<Index>>
struct graph_traits<Outer<Inner<Index>>> {
  using vertex_id_type = Index;
};


// The following concepts are to capture some general concrete graphs that are used: 
// range of range of vertex and range of range of tuples

template <typename R>
concept vertex_list_c = std::ranges::forward_range<R> && !std::is_compound_v<std::ranges::range_value_t<R>>;

template <typename R>
concept edge_list_c = std::ranges::forward_range<R> && requires(std::ranges::range_value_t<R> e) {
  std::get<0>(e);
};

template <typename R>
concept property_edge_list_c = std::ranges::forward_range<R> && requires(std::ranges::range_value_t<R> e) {
  std::get<1>(e);
};

template <typename G>
concept min_idx_adjacency_list = 
     std::ranges::random_access_range<G>
  && vertex_list_c<inner_range_t<G>>
  && std::is_convertible_v<inner_value_t<G>, std::ranges::range_difference_t<G>>
  && requires(G g, inner_value_t<G> u) {
     { g[u] } -> std::convertible_to<inner_range_t<G>>;
};

template <typename G>
concept idx_adjacency_list = 
     std::ranges::random_access_range<G>
  && edge_list_c<inner_range_t<G>>
  && std::is_convertible_v<std::tuple_element_t<0, inner_value_t<G>>, std::ranges::range_difference_t<G>>
  && requires(G g, std::tuple_element_t<0, inner_value_t<G>> u) {
  { g[u] } -> std::convertible_to<inner_range_t<G>>;
};


// Based on the above concepts, we define concept-based overloads for vertex_id_type and some CPOs

// Graph traits
template <min_idx_adjacency_list G>
struct graph_traits<G> {
  using vertex_id_type = inner_value_t<G>;
};

template <idx_adjacency_list G>
struct graph_traits<G> {
  using vertex_id_type = std::tuple_element_t<0, inner_value_t<G>>;
};

// target CPO
template <idx_adjacency_list T, class U>
auto tag_invoke(const target_tag, const T& graph, const U& e) {
  return std::get<0>(e);
}

template <min_idx_adjacency_list T, class U>
auto tag_invoke(const target_tag, const T& graph, const U& e) {
  return e;
}

// num_vertices CPO
template <idx_adjacency_list T>
auto tag_invoke(const num_vertices_tag, const T& graph) {
  return graph.size();
}

template <min_idx_adjacency_list T>
auto tag_invoke(const num_vertices_tag, const T& graph) {
  return graph.size();
}




}    // namespace nw::graph

#endif    //  NW_GRAPH_GRAPH_CONCEPTS_HPP
