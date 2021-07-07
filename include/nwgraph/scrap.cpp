// #include "nwgraph/graph_base.hpp"

#if 0

template <typename T>
using inner_range = std::ranges::range_value_t<T>;

template <typename T>
using inner_value = std::ranges::range_value_t<inner_range<T>>;

template <typename T>
using vertex_id_type = nw::graph::vertex_id_type;

template <typename T>
using index_t = nw::graph::vertex_id_type;

template <typename T>
concept Adjacence =
  requires (T graph, inner_value<T> vertex) {
    { target (vertex) } -> std::convertible_to<vertex_id_type<T>>;
  };

template <typename T>
concept Incidence =
  requires (T graph, inner_value<T> edge) {
    { source (edge) } -> std::convertible_to<vertex_id_type<T>>;
    { target (edge) } -> std::convertible_to<vertex_id_type<T>>;
  };

template <typename T>
concept Graph = 
  std::ranges::random_access_range<T> &&
  std::ranges::forward_range<inner_range<T>> &&
  std::convertible_to<vertex_id_type<T>, index_t<T>>;

template <typename T>
concept IncidenceGraph = Graph<T> && Incidence<T>;

template <typename T>
concept AdjacenceGraph = Graph<T> && Adjacence<T>;


namespace nw {
namespace graph {

template <typename E>
concept edge_list_c = std::ranges::random_access_range<E>&& requires(E e) {
  typename E::vertex_id_type;
  typename E::attributes_t;
  typename E::element;
  e.size();
  { std::is_same_v<decltype(E::edge_directedness), directedness> };

};

}    // namespace graph
}    // namespace nw
#endif

template <typename G>
auto num_edges(const G& g) {
  return g.num_edges();
}

template <typename G>
auto begin(const G& g) {
  return g.begin();
}

template <typename G>
auto num_vertices(const G& g) {
  return g.num_vertices();
}
