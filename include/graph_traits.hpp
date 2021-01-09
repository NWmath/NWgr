

#ifndef NW_GRAPH_GRAPH_TRAITS_HPP
#define NW_GRAPH_GRAPH_TRAITS_HPP

namespace nw {
namespace graph {

template <typename G>
struct graph_traits {

  using vertex_id_t    = typename G::vertex_id_t;
  using vertex_size_t  = typename G::vertex_id_t;
  using num_vertices_t = typename G::num_vertices_t;
  using num_edges_t    = typename G::num_edges_t;

  // using outer_iterator = typename G::outer_iterator;
  // using inner_iterator = typename G::inner_iterator;
};



}
}

#endif    // NW_GRAPH_GRAPH_TRAITS_HPP