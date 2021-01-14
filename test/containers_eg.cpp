

#include <deque>
#include <vector>
#include <forward_list>
#include <tuple>


#include "graph_traits.hpp"

#include "containers/volos.hpp"
#include "containers/edge_list.hpp"
#include "containers/adjacency.hpp"
#include "containers/vovos.hpp"


#include "graph_concepts.hpp"

template <nw::graph::edge_list_graph edge_list_t>
auto t0 (edge_list_t el) {
}

template <nw::graph::edge_list_graph edge_list_t>
auto t1 (const edge_list_t& el) {
}

template <nw::graph::edge_list_graph edge_list_t>
auto t2 (edge_list_t&& el) {
}


template <nw::graph::adjacency_graph adjacency_t>
auto t3 (adjacency_t el) {
}

template <nw::graph::adjacency_graph adjacency_t>
auto t4 (const adjacency_t& el) {
}

template <nw::graph::adjacency_graph adjacency_t>
auto t5 (adjacency_t&& el) {
}



template <nw::graph::adjacency_graph Graph>
auto bfs_vv(const Graph& graph, typename nw::graph::graph_traits<Graph>::vertex_id_t root) {
  using vertex_id_t = typename nw::graph::graph_traits<Graph>::vertex_id_t;

  std::deque<vertex_id_t>  q1, q2;
  std::vector<vertex_id_t> level(graph.num_vertices()[0], std::numeric_limits<vertex_id_t>::max());
  std::vector<vertex_id_t> parents(graph.num_vertices()[0], std::numeric_limits<vertex_id_t>::max());
  size_t                   lvl = 0;

  q1.push_back(root);
  level[root]   = lvl++;
  parents[root] = root;

  auto g = graph.begin();

  while (!q1.empty()) {

    std::for_each(q1.begin(), q1.end(), [&](vertex_id_t u) {
      std::for_each(g[u].begin(), g[u].end(), [&](auto&& x) {
        vertex_id_t v = std::get<0>(x);
        if (level[v] == std::numeric_limits<vertex_id_t>::max()) {
          q2.push_back(v);
          level[v]   = lvl;
          parents[v] = u;
        }
      });
    });
    std::swap(q1, q2);
    q2.clear();
    ++lvl;
  }
  return parents;
}


template <typename T>
void adjacency_container_test(T a) {
  static_assert(std::is_constructible_v<T>);
  static_assert(std::is_default_constructible_v<typename T::outer_iterator>);
  static_assert(std::is_copy_constructible_v<typename T::outer_iterator>);
  static_assert(std::is_move_constructible_v<typename T::outer_iterator>);
  static_assert(std::is_constructible_v<typename T::outer_iterator>);
}



template <typename T>
void adjacency_concept_test(T a) {

  static_assert(nw::graph::adjacency_graph<T>);

  t3(a);
  t4(a);
  t5(std::move(a));
}


int main() {

  //  nw::graph::edge_list a { {0, 0}, {0, 4} };  // compiler dumps core

  nw::graph::edge_list<nw::graph::directedness::directed> e { { 0, 0}, { 0, 4}, {4, 4}, {4, 0} };

  static_assert(std::is_constructible_v<decltype(e)>);
  static_assert(std::ranges::forward_range<nw::graph::edge_list<nw::graph::directedness::directed>>);

  static_assert(nw::graph::edge_list_graph<decltype(e)>);

  t0(e);
  t1(e);
  t2(std::move(e));


  adjacency_concept_test(std::vector<std::forward_list<std::tuple<int, int, double>>>());

  adjacency_concept_test(nw::graph::adjacency<0>());
  // auto a = make_adjacency<0>(e);
  // adjacency_concept_test(a);


  adjacency_concept_test(nw::graph::adj_list<0>());

  
  bfs_vv(nw::graph::adj_list<0>(), 0);

  // bfs_vv(a, 0);

  return 0;
}
