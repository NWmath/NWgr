
#if 0
template <typename G>
concept AdjacencyGraphAA = 
  requires(G g, size_t i) {
    typename G::iterator;
    { g.begin() } -> std::convertible_to<typename G::iterator>;
    { g.end()   } -> std::convertible_to<typename G::iterator>;
    { g[i]      } -> std::convertible_to<typename G::iterator::value_type>;
    { g.size()  } -> std::convertible_to<size_t>;
};

#include <ranges>


template <typename G>
concept AdjacencyGraph = std::ranges::random_access_range<G>;


template< class T >
concept _range = requires(T& t) {
  t.begin();
  t.end();
};

template< class T >
concept _sized_range = _range<T> &&
  requires(T& t) {
    t.size();
  };

template<class T>
concept _input_range =
  _range<T> ; // && std::input_iterator<typename T::iterator>;

template<class T>
concept _forward_range =
  _input_range<T> ; // && std::forward_iterator<typename T::iterator>;

template<class T>
concept _bidirectional_range =
  _forward_range<T> ; // && std::bidirectional_iterator<typename T::iterator>;

template<class T>
concept _random_access_range =
  _bidirectional_range<T> ; //  && std::random_access_iterator<typename T::iterator>;

  // outer_iterator ... failing is_constructible<>


template<typename G>
concept AdjacencyGraph = ranges::random_access_range<G> && 
  ranges::forward_range<typename G::iterator::value_type> &&
  std::is_convertible_v<typename G::iterator::value_type::iterator::value_type, typename G::index_type>;
  // value_type of inner_range is a tuple
  //   edge? -- tuple is source, target, edge properties
  //   vertex? -- tuple is target, vertex properties -- or are vertex properties always indexable (random_access_range)?
  

  // In original Boost.Graph
  //   AdjacencyGraph => inner container is vertices (rarely used)
  //   IncidenceGraph => inner container is edges
  //     with Incidence Graph -- one can get neighbor vertices and vertex properties if vertex properties are indexable
  
template<typename G>
concept IncidenceGraph = ranges::random_access_range<G> && 
  ranges::forward_range<typename G::iterator::value_type> &&
  std::is_same_v<typename G::iterator::value_type::iterator::value_type, std::tuple<vertex_id_t,vertex_id_t, EdgeProps...>>;
  // Still need to be able store this as CSR
  // Adapt to edge_range


template<typename G>
concept TopologyGraph = ranges::random_access_range<G> && 
  ranges::forward_range<typename G::iterator::value_type> &&
  std::is_same_v<typename G::iterator::value_type::iterator::value_type, std::tuple<vertex_id_t, VertProp??...>>;

  //   I. Conceptify interfaces -> IncidenceGraph
  //      A. const 
  //  II. Refactor algorithms to use IncidenceGraph
  // III. Maybe not use std::get<0> and std::get<1> for accessing edge information (source/target?)
  //  IV. Maybe rather than begin()/end() -> neighbors()? or out_edges()?
#endif

#include <ranges>
#include "vovos.hpp"
#include "aolos.hpp" 
#include "compressed.hpp" 

#include <concepts>

template <class T>
concept Graph = std::ranges::random_access_range<T>;

template <class T>
auto foo(const T& A) requires Graph<T> {

  auto _b = std::ranges::begin(A);
  auto _e = std::ranges::end(A);
  auto _f = A[0];

  auto _b_b = std::ranges::begin(*_b);
  auto _b_e = std::ranges::end(*_b);

  for (auto& j : A) {
    for (auto& k : j) {

    }
  }

  for (auto& j : A[0]) {

  }
}


int main() {

  foo(nw::graph::vector_of_vector_of_structs(5));
  foo(nw::graph::vov<>(5));


  foo(nw::graph::array_of_list_of_structs(5));
  foo(nw::graph::adj_list(5));



  // foo<nw::graph::adjacency<0>>();


  return 0;
}
