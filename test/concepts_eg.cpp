

#include <deque>
#include <vector>
#include <tuple>



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

  static_assert(edge_list_c<decltype(e)>);
  static_assert(edge_list_graph<decltype(e)>);

  auto a = make_adjacency<0>(e);


  
  static_assert(nw::graph::struct_of_arrays_c<std::tuple<std::vector<int>>>);


  bfs_vv(a, 0);

  return 0;
}
