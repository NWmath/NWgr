

#include <deque>
#include <vector>
#include <tuple>



#include "nwgraph/vovos.hpp"

#include "nwgraph/adjacency.hpp"
#include "nwgraph/edge_list.hpp"

#include "nwgraph/graph_concepts.hpp"


template <nw::graph::edge_list_c edge_list_t>
auto foo(edge_list_t el) {}

template <nw::graph::edge_list_c edge_list_t>
auto bar(const edge_list_t& el) {}

template <nw::graph::edge_list_c edge_list_t>
auto baz(edge_list_t&& el) {}

template <nw::graph::adjacency_graph Graph>
auto bfs_vv(const Graph& graph, typename nw::graph::graph_traits<Graph>::vertex_id_type root) {
  using vertex_id_type = typename nw::graph::graph_traits<Graph>::vertex_id_type;

  std::deque<vertex_id_type>  q1, q2;
  std::vector<vertex_id_type> level(graph.num_vertices()[0], std::numeric_limits<vertex_id_type>::max());
  std::vector<vertex_id_type> parents(graph.num_vertices()[0], std::numeric_limits<vertex_id_type>::max());
  size_t                      lvl = 0;

  q1.push_back(root);
  level[root]   = lvl++;
  parents[root] = root;

  auto g = graph.begin();

  while (!q1.empty()) {

    std::for_each(q1.begin(), q1.end(), [&](vertex_id_type u) {
      std::for_each(g[u].begin(), g[u].end(), [&](auto&& x) {
        vertex_id_type v = std::get<0>(x);
        if (level[v] == std::numeric_limits<vertex_id_type>::max()) {
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

int main() {


  nw::graph::edge_list<nw::graph::directedness::directed> e{{0, 0}, {0, 4}, {4, 4}, {4, 0}};

  std::vector<std::tuple<int, int>> f;
  std::tuple<std::vector<int>, std::vector<int>> g;


  static_assert(nw::graph::edge_list_c<decltype(e)>);
  static_assert(nw::graph::edge_list_graph<decltype(e)>);

  static_assert(nw::graph::edge_list_c<decltype(f)>);
  // static_assert(nw::graph::edge_list_c<decltype(g)>);

  auto a = nw::graph::make_adjacency<0>(e);
  // auto b = nw::graph::make_adjacency<0>(f, 2UL);
  // auto c = nw::graph::make_adjacency<0>(g, 2UL);


  
  static_assert(nw::graph::struct_of_arrays_c<std::tuple<std::vector<int>>>);


  bfs_vv(a, 0);

  return 0;
}
