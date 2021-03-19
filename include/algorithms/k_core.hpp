//
// This file is part of Standard Graph Library (SGL)
// (c) Pacific Northwest National Laboratory 2018
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0
// International License https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Kevin Deweese
//

#ifndef NW_GRAPH_K_CORE_HPP
#define NW_GRAPH_K_CORE_HPP

#include "adaptors/edge_range.hpp"
#include "adaptors/new_dfs_range.hpp"
#include "util/defaults.hpp"
#include "util/util.hpp"
#include <algorithm>
#include <tuple>
#include <unordered_map>
#include <utility>

namespace nw {
namespace graph {

struct pair_hash {
  std::size_t operator()(const std::pair<size_t, size_t>& p) const {

    auto h1 = std::hash<default_vertex_id_type>{}(p.first);
    auto h2 = std::hash<default_vertex_id_type>{}(p.second);

    return h1 ^ h2;
  }
};
using Neighbors     = std::pair<size_t, size_t>;
using Unordered_map = std::unordered_map<Neighbors, bool, pair_hash>;

Neighbors make_my_pair(default_vertex_id_type x, default_vertex_id_type y) {
  if (x < y) return std::make_pair(x, y);
  return std::make_pair(y, x);
}

//****************************************************************************
template <typename Graph>
std::tuple<Unordered_map, size_t> k_core(const Graph& A, int k) {
  Unordered_map filter;

  size_t                          n_vtx     = A.size();
  size_t                          old_n_vtx = n_vtx;
  std::vector<int>                degree(n_vtx, 0);
  std::vector<vertex_id_t<Graph>> relabel(n_vtx);

  for (auto&& [u, v] : edge_range(A)) {
    ++degree[v];
  }

  dfs_edge_range ranges(A, 0);
  auto           ite = ranges.begin();
  for (; ite != ranges.end(); ++ite) {
    auto v = std::get<1>(*ite);
    auto w = std::get<2>(*ite);

    Neighbors edge = make_my_pair(v, w);

    if (filter.find(edge) == filter.end() && degree[v] < k) {
      if (--degree[w] == 0) n_vtx--;
      if (--degree[v] == 0) n_vtx--;

      filter.insert({edge, true});
    }
  }

  /*vertex_id_type new_label = 0;
  for(size_t i = 0; i < old_n_vtx; ++i) {
    if(degree[i] != 0){
      relabel[i] = new_label;
      new_label++;
    }
    }*/

  // edge_list<undirected> k_core_list(n_vtx);
  // Graph k_core_graph(n_vtx);
  // k_core_graph.open_for_pushback();
  // first = A.begin();
  // last = A.end();

  /*for(auto G=first; first !=last; ++first) {
    for (auto v = (*first).begin(); v != (*first).end(); ++v) {
      Neighbors edge = make_my_pair(first-G,std::get<0>(*v));
      if(filter.find(edge) != filter.end())
    std::cout << "removed edge " << first-G << " " << std::get<0>(*v) << " " <<
  std::endl; else k_core_graph.push_back(relabel[first-G],v);
    }
  }
  */

  return std::make_tuple(filter, n_vtx);
}

}    // namespace graph
}    // namespace nw
#endif
