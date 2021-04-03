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


template <typename G>
using inner_range = std::ranges::range_value_t<G>;


template <typename G>
using inner_value = std::ranges::range_value_t<inner_range<G>>;


template <class G>
concept struct_of_arrays_c = std::semiregular<G> && requires(G g) {
  std::apply([](auto&&... elts){  (elts[0],...); }, g);
};


template <typename G>
concept graph = std::semiregular<G>&& requires(G g) {
  typename vertex_id_t<G>;
  { num_vertices(g) } -> std::convertible_to<size_t>;
};


template <typename G>
concept edge_enumerable_graph = graph<G>&& requires(G g) {
  { num_edges(g) } -> std::convertible_to<size_t>;
};


template <typename R>
concept vertex_list_c = std::ranges::forward_range<R> &&
  requires(std::ranges::range_value_t<R> e) {
  std::get<0>(e);
};


template <typename R, typename G = R>
concept edge_list_c = std::ranges::forward_range<G> &&
  requires(std::ranges::range_value_t<R> e) {
  std::get<0>(e); 
  std::get<1>(e); 
};


template <typename G>
concept edge_list_graph = edge_enumerable_graph<G> && edge_list_c<G> &&
  requires(std::ranges::range_value_t<G> e) {
  { std::get<0>(e) } -> std::convertible_to<vertex_id_t<G>>;
  { std::get<1>(e) } -> std::convertible_to<vertex_id_t<G>>;
};


template <typename G>
concept adjacency_graph = graph<G> && std::ranges::random_access_range<G> && vertex_list_c<inner_range<G>> &&
  requires(G g, inner_value<G> e, vertex_id_t<G> u) {
  { g[u] } -> std::same_as<inner_range<G>>;
  { std::get<0>(e) } -> std::convertible_to<vertex_id_t<G>>;  
};


template <typename G>
concept key_adjacency_graph = graph<G> && std::ranges::random_access_range<G> && vertex_list_c<inner_range<G>> &&
  requires(G g, inner_value<G> e, vertex_id_t<G> u) {
  { g[u] } -> std::same_as<std::tuple<vertex_id_t<G>, inner_range<G>>>;
  { std::get<0>(e) } -> std::convertible_to<vertex_id_t<G>>;
};


template <typename G>
concept degree_enumerable_graph = adjacency_graph<G> &&
  requires(G g, vertex_id_t<G> u) {
  degrees<0>(g);
  degrees<1>(g);
  degree(g, u);
};

  
}    // namespace nw::graph

#endif    //  NW_GRAPH_GRAPH_CONCEPTS_HPP
