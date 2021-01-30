

#ifndef NW_GRAPH_GRAPH_TRAITS_HPP
#define NW_GRAPH_GRAPH_TRAITS_HPP

namespace nw {
namespace graph {

template <typename G>
struct graph_traits {

  using vertex_id_type    = typename G::vertex_id_type;
  using vertex_size_type  = typename G::vertex_id_type;
  using num_vertices_type = typename G::num_vertices_type;
  using num_edges_type    = typename G::num_edges_type;

  // using outer_iterator = typename G::outer_iterator;
  // using inner_iterator = typename G::inner_iterator;
};

template <typename G>
using vertex_id_t = typename graph_traits<G>::vertex_id_type;

template <typename G>
using vertex_size_t = typename graph_traits<G>::vertex_size_type;

template <typename G>
using num_vertices_t = typename graph_traits<G>::num_vertices_type;

template <typename G>
using num_edges_t = typename graph_traits<G>::num_edges_type;


}    // namespace graph
}    // namespace nw

#endif    // NW_GRAPH_GRAPH_TRAITS_HPP
