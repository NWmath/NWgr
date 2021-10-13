//
// This file is part of BGL17 (aka NWGraph aka GraphPack aka the Graph Standard Library)
// (c) Pacific Northwest National Laboratory 2018
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#include <deque>
#include <tuple>
#include <vector>

#include "nwgraph/adjacency.hpp"
#include "nwgraph/edge_list.hpp"
#include "nwgraph/graph_concepts.hpp"
#include "nwgraph/vovos.hpp"

template <nw::graph::edge_list_c edge_list_t>
auto foo(edge_list_t el) {}

template <nw::graph::edge_list_c edge_list_t>
auto bar(const edge_list_t& el) {}

template <nw::graph::edge_list_c edge_list_t>
auto baz(edge_list_t&& el) {}

template <nw::graph::adjacency_list_graph Graph>
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
  auto                              g = nw::graph::make_zipped(std::vector<int>(), std::vector<int>());

  static_assert(nw::graph::edge_list_c<decltype(e)>);
  static_assert(nw::graph::edge_list_graph<decltype(e)>);
  static_assert(nw::graph::edge_list_c<decltype(f)>);
  static_assert(nw::graph::edge_list_c<decltype(g)>);

  auto h = nw::graph::make_adjacency<0>(e);
  static_assert(nw::graph::adjacency_list_graph<decltype(h)>);
  static_assert(nw::graph::edge_list_c<decltype(h[0])>);

  bfs_vv(h, 0);

  return 0;
}
