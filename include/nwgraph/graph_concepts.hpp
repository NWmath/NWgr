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


// template <std::ranges::random_access_range G>
// std::array<typename G::size_type, 1> num_vertices(const G& g) {
//  return {size(g)};
// }

// }    // namespace graph
// }    // namespace nw

namespace nw {
namespace graph {

template <typename G>
using inner_range = typename std::iterator_traits<typename G::iterator>::value_type;
// using inner_range = std::ranges::range_value_t<G>;

template <typename G>
using inner_value = typename std::iterator_traits<typename inner_range<G>::iterator>::value_type;
// using inner_value = std::ranges::range_value_t<inner_range<G>>;


template <typename G>
concept struct_of_arrays_c = std::semiregular<G> && requires(G g) {
  std::get<0>(g);
};

template <typename G>
concept graph = std::semiregular<G>&& requires(G g) {
  typename graph_traits<G>::vertex_id_type;
  typename graph_traits<G>::num_vertices_type;
  { num_vertices(g) }
  ->std::convertible_to<size_t>;
  //  ->std::convertible_to<typename graph_traits<G>::num_vertices_type>;
};

template <typename G>
concept edge_enumerable_graph = graph<G>&& requires(G g) {
  typename graph_traits<G>::num_edges_type;
  { num_edges(g) }
  ->std::convertible_to<typename graph_traits<G>::num_edges_type>;
};

template <typename R, typename G = R>
concept vertex_list_c = std::forward_iterator<typename R::iterator>
                        // std::ranges::forward_range<R>
                        && (
                               requires(typename std::iterator_traits<typename R::iterator>::value_type e) {
                                 { std::get<0>(e) }
                                 ->std::convertible_to<typename graph_traits<G>::vertex_id_type>;    // target
                               } ||
                               requires(typename std::iterator_traits<typename R::iterator>::value_type e) {
                                 { adjacent(e) }
                                 ->std::convertible_to<typename graph_traits<G>::vertex_id_type>;
                               });

template <typename R, typename G = R>
concept edge_list_c = std::forward_iterator<typename G::iterator> &&
                      (
                          requires(typename std::iterator_traits<typename R::iterator>::value_type e) {
                            { std::get<0>(e) }
                            ->std::convertible_to<typename graph_traits<G>::vertex_id_type>;    // source
                            { std::get<1>(e) }
                            ->std::convertible_to<typename graph_traits<G>::vertex_id_type>;    // target
                          } ||
                          requires(typename std::iterator_traits<typename R::iterator>::value_type e) {
                            { source(e) }
                            ->std::convertible_to<typename graph_traits<G>::vertex_id_type>;
                            { target(e) }
                            ->std::convertible_to<typename graph_traits<G>::vertex_id_type>;
                          });

template <typename G>
concept edge_list_graph = graph<G>&& edge_list_c<G>;

template <typename G>
concept adjacency_graph = graph<G>&& std::random_access_iterator<typename G::iterator>&& vertex_list_c<inner_range<G>, G> &&
                          (
                              requires(G g, typename graph_traits<G>::vertex_id_type u) {
                                { adjacent_vertices(g, u) }
                                ->std::convertible_to<inner_range<G>>;
                              } ||
                              requires(G g, typename graph_traits<G>::vertex_id_type u) {
                                { g[u] }
                                ->std::convertible_to<inner_range<G>>;    // from forward_range?
                              });

template <typename G>
concept incidence_graph = graph<G>&& std::random_access_iterator<typename G::iterator>&& edge_list_c<inner_range<G>, G> &&
                          (
                              requires(G g, typename graph_traits<G>::vertex_id_type u) {
                                { out_edges(g, u) }
                                ->std::convertible_to<inner_range<G>>;
                              } ||
                              requires(G g, typename graph_traits<G>::vertex_id_type u) {
                                { g[u] }
                                ->std::convertible_to<inner_range<G>>;    // from forward_range?
                              });

template <typename G>
concept degree_enumerable_graph = (adjacency_graph<G> || incidence_graph<G>)
  && (
         requires(G g, typename graph_traits<G>::vertex_id_type u) {
	{ degree(g, u) }
	  ->std::convertible_to<size_t>;
      } 
      || requires(G g, typename graph_traits<G>::vertex_id_type u) {
	{ degree(g[u]) }
	  ->std::convertible_to<size_t>;
      }
      );

}    // namespace graph
}    // namespace nw

#endif    //  NW_GRAPH_GRAPH_CONCEPTS_HPP
