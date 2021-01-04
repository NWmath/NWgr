

#ifndef NW_GRAPH_GRAPH_CONCEPTS_HPP
#define NW_GRAPH_GRAPH_CONCEPTS_HPP

#include <concepts>
#include <ranges>

// #include "graph_base.hpp"

#if 0

template <typename T>
using inner_range = std::ranges::range_value_t<T>;

template <typename T>
using inner_value = std::ranges::range_value_t<inner_range<T>>;

template <typename T>
using vertex_id_t = nw::graph::vertex_id_t;

template <typename T>
using index_t = nw::graph::vertex_id_t;

template <typename T>
concept Adjacence =
  requires (T graph, inner_value<T> vertex) {
    { target (vertex) } -> std::convertible_to<vertex_id_t<T>>;
  };

template <typename T>
concept Incidence =
  requires (T graph, inner_value<T> edge) {
    { source (edge) } -> std::convertible_to<vertex_id_t<T>>;
    { target (edge) } -> std::convertible_to<vertex_id_t<T>>;
  };

template <typename T>
concept Graph = 
  std::ranges::random_access_range<T> &&
  std::ranges::forward_range<inner_range<T>> &&
  std::convertible_to<vertex_id_t<T>, index_t<T>>;

template <typename T>
concept IncidenceGraph = Graph<T> && Incidence<T>;

template <typename T>
concept AdjacenceGraph = Graph<T> && Adjacence<T>;


namespace nw {
namespace graph {

template <typename E>
concept edge_list_c = std::ranges::random_access_range<E>&& requires(E e) {
  typename E::vertex_id_t;
  typename E::attributes_t;
  typename E::element;
  e.size();
  { std::is_same_v<decltype(E::edge_directedness), directedness> };

};

}    // namespace graph
}    // namespace nw
#endif

#include "graph_traits.hpp"

namespace nw {
namespace graph {


template <typename G>
using inner_range = std::ranges::range_value_t<G>;


template <typename G>
using inner_value = std::ranges::range_value_t<inner_range<G>>;


template <typename G>
concept graph = std::semiregular<G>&& requires(G g) {
  graph_traits<G>::vertex_id_t;
  graph_traits<G>::num_vertices_t;
  graph_traits<G>::num_edges_t;

  { num_vertices(g) }
  ->std::convertible_to<typename graph_traits<G>::num_vertices_t>;
  { num_edges(g) }
  ->std::convertible_to<typename graph_traits<G>::num_edges_t>;
};


template <typename G>
concept vertex_list_c = std::ranges::forward_range<G> &&
                      (
                          requires(G g, std::ranges::range_value_t<G> e) {
                            { std::get<0>(e) }
                            ->std::convertible_to<typename graph_traits<G>::vertex_id_t>;    // target
                          } ||
                          requires(G g, std::ranges::range_value_t<G> e) {
                            { target(e) }
                            ->std::convertible_to<typename graph_traits<G>::vertex_id_t>;
                          });

template <typename G>
concept edge_list_c = std::ranges::forward_range<G> &&
                    (
                        requires(G g, std::ranges::range_value_t<G> e) {
                          { std::get<0>(e) }
                          ->std::convertible_to<typename graph_traits<G>::vertex_id_t>;    // source
                          { std::get<1>(e) }
                          ->std::convertible_to<typename graph_traits<G>::vertex_id_t>;    // target
                        } ||
                        requires(G g, std::ranges::range_value_t<G> e) {
                          { source(e) }
                          ->std::convertible_to<typename graph_traits<G>::vertex_id_t>;
                          { target(e) }
                          ->std::convertible_to<typename graph_traits<G>::vertex_id_t>;
                        });


template <typename G>
concept edge_list_graph = graph<G>&& edge_list_c<G>;


template <typename G>
concept adjacency_graph = graph<G>&& std::ranges::random_access_range<G>&& vertex_list_c<inner_range<G>> &&
                          (
                              requires(G g, graph_traits<G>::vertex_id_t u) {
                                { adjacent_vertices(g, u) }
                                ->std::convertible_to<inner_range<G>>;
                              } ||
                              requires(G g, graph_traits<G>::vertex_id_t u) {
                                { g[u] }
                                ->std::convertible_to<inner_range<G>>;    // from forward_range?
                              });

template <typename G>
concept incidence_graph = graph<G>&& std::ranges::random_access_range<G>&& edge_list_c<inner_range<G>> &&
                          (
                              requires(G g, graph_traits<G>::vertex_id_t u) {
                                { out_edges(g, u) }
                                ->std::convertible_to<inner_range<G>>;
                              } ||
                              requires(G g, graph_traits<G>::vertex_id_t u) {
                                { g[u] }
                                ->std::convertible_to<inner_range<G>>;    // from forward_range?
                              });

}    // namespace graph
}    // namespace nw

#endif    //  NW_GRAPH_GRAPH_CONCEPTS_HPP
