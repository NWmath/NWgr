//
// This file is part of BGL17 NW Graph aka GraphPack aka the Graph Standard Library
// (c) Pacific Northwest National Laboratory 2018-2020
// (c) University of Washington 2018-2020
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//


#ifndef NW_GRAPH_GRAPH_CONCEPTS_HPP
#define NW_GRAPH_GRAPH_CONCEPTS_HPP

#include <concepts>
#include <iterator>
#include <tuple>

#include "graph_traits.hpp"

namespace nw {
namespace graph {


template <typename G>
using inner_range = std::iterator_traits<typename G::iterator>::value_type;
// using inner_range = std::ranges::range_value_t<G>;

template <typename G>
using inner_value = std::iterator_traits<typename inner_range<G>::iterator>::value_type;
// using inner_value = std::ranges::range_value_t<inner_range<G>>;

template <typename G>
concept graph = std::semiregular<G>&& requires(G g) {
  typename graph_traits<G>::vertex_id_t;
  typename graph_traits<G>::num_vertices_t;

  { num_vertices(g) }
  ->std::convertible_to<typename graph_traits<G>::num_vertices_t>;

  //  typename graph_traits<G>::num_edges_t;
  //  { num_edges(g) }
  //  ->std::convertible_to<typename graph_traits<G>::num_edges_t>;
};

template <typename R, typename G = R>
concept vertex_list_c = std::forward_iterator<typename R::iterator>
                        // std::ranges::forward_range<R>
                        && (
                               requires(std::iterator_traits<typename R::iterator>::value_type e) {
                                 { std::get<0>(e) }
                                 ->std::convertible_to<typename graph_traits<G>::vertex_id_t>;    // target
                               } ||
                               requires(std::iterator_traits<typename R::iterator>::value_type e) {
                                 { adjacent(e) }
                                 ->std::convertible_to<typename graph_traits<G>::vertex_id_t>;
                               });

template <typename R, typename G = R>
concept edge_list_c = std::forward_iterator<typename G::iterator> &&
                      (
                          requires(std::iterator_traits<typename R::iterator>::value_type e) {
                            { std::get<0>(e) }
                            ->std::convertible_to<typename graph_traits<G>::vertex_id_t>;    // source
                            { std::get<1>(e) }
                            ->std::convertible_to<typename graph_traits<G>::vertex_id_t>;    // target
                          } ||
                          requires(std::iterator_traits<typename R::iterator>::value_type e) {
                            { source(e) }
                            ->std::convertible_to<typename graph_traits<G>::vertex_id_t>;
                            { target(e) }
                            ->std::convertible_to<typename graph_traits<G>::vertex_id_t>;
                          });

template <typename G>
concept edge_list_graph = graph<G>&& edge_list_c<G>;

template <typename G>
concept adjacency_graph = graph<G>&& std::random_access_iterator<typename G::iterator>&& vertex_list_c<inner_range<G>, G> &&
                          (
                              requires(G g, typename graph_traits<G>::vertex_id_t u) {
                                { adjacent_vertices(g, u) }
                                ->std::convertible_to<inner_range<G>>;
                              } ||
                              requires(G g, typename graph_traits<G>::vertex_id_t u) {
                                { g[u] }
                                ->std::convertible_to<inner_range<G>>;    // from forward_range?
                              });

template <typename G>
concept incidence_graph = graph<G>&& std::random_access_iterator<typename G::iterator>&& edge_list_c<inner_range<G>, G> &&
                          (
                              requires(G g, typename graph_traits<G>::vertex_id_t u) {
                                { out_edges(g, u) }
                                ->std::convertible_to<inner_range<G>>;
                              } ||
                              requires(G g, typename graph_traits<G>::vertex_id_t u) {
                                { g[u] }
                                ->std::convertible_to<inner_range<G>>;    // from forward_range?
                              });

}    // namespace graph
}    // namespace nw

#endif    //  NW_GRAPH_GRAPH_CONCEPTS_HPP
