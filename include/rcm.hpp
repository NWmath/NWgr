//
// This file is part of BGL17 (aka NWGraph aka GraphPack aka the Graph Standard Library)
// (c) Pacific Northwest National Laboratory 2018
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Kevin Deweese
//

#ifndef NW_GRAPH_RCM_HPP
#define NW_GRAPH_RCM_HPP

#include "edge_list.hpp"
#include "util/types.hpp"
#include "util/AtomicBitVector.hpp"
#include "util/parallel_for.hpp"
#include <tbb/concurrent_vector.h>
#include <tbb/parallel_for_each.h>

namespace nw {
namespace graph {

template <typename... Attributes>
std::vector<vertex_id_t> rcm(edge_list<directed, Attributes...>& el, vertex_id_t seed = std::numeric_limits<vertex_id_t>::max()) {
  auto degree = el.degrees();
  auto N      = degree.size();

  vertex_id_t                       vtx = std::get<0>(*(el.begin()));
  std::vector<decltype(el.begin())> index;
  index.push_back(el.begin());
  for (auto it = el.begin(); it != el.end(); ++it) {
    if (std::get<0>(*it) > vtx) {
      vtx = std::get<0>(*it);
      index.push_back(it);
    }
  }
  index.push_back(el.end());

  std::vector<vertex_id_t> perm;
  perm.reserve(N);
  nw::graph::AtomicBitVector              marked(N);
  tbb::concurrent_vector<vertex_id_t> q1, q2;

  if (seed == std::numeric_limits<vertex_id_t>::max()) {
    seed = std::min_element(degree.begin(), degree.end()) - degree.begin();
  }
  marked.atomic_set(seed);
  perm.push_back(seed);
  q1.push_back(seed);

  while (perm.size() < N) {
    std::for_each(std::execution::par_unseq, q1.begin(), q1.end(), [&](vertex_id_t u) {
      std::for_each(std::execution::par_unseq, index[u], index[u + 1], [&](auto&& x) {
        vertex_id_t v = std::get<1>(x);
        if (marked.atomic_get(v) == 0 && marked.atomic_set(v) == 0) {
          q2.push_back(v);
        }
      });
    });

    std::sort(q2.begin(), q2.end(), [&](auto&& a, auto&& b) { return degree[a] < degree[b]; });
    perm.insert(perm.end(), q2.begin(), q2.end());
    std::swap(q1, q2);
    q2.clear();
  }
  std::reverse(std::execution::par_unseq, perm.begin(), perm.end());
  return perm;
}

}    // namespace graph
}    // namespace nw

#endif    // NW_GRAPH_RCM_HPP
