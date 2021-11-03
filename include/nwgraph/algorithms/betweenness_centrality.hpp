// 
// This file is part of NW Graph (aka GraphPack) 
// (c) Pacific Northwest National Laboratory 2018-2021 
// (c) University of Washington 2018-2021 
// 
// Licensed under terms of include LICENSE file 
// 
// Authors: 
//     Andrew Lumsdaine	
//     Kevin Deweese	
//     Luke D'Alessandro	
//

#ifndef BETWEENNESS_CENTRALITY_HPP
#define BETWEENNESS_CENTRALITY_HPP

#include "nwgraph/adaptors/worklist.hpp"
#include "nwgraph/util/AtomicBitVector.hpp"
#include "nwgraph/util/atomic.hpp"
#include "nwgraph/util/parallel_for.hpp"
#include "nwgraph/util/util.hpp"

#include <algorithm>

#if defined(CL_SYCL_LANGUAGE_VERSION)
#include <dpstd/algorithm>
#include <dpstd/execution>
#include <dpstd/numeric>
#else
#include <execution>
#endif

#include <forward_list>
#include <future>
#include <iostream>
#include <list>
#include <mutex>
#include <queue>
#include <set>
#include <stack>
#include <thread>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include <tbb/concurrent_queue.h>
#include <tbb/concurrent_vector.h>
#include <tbb/parallel_for_each.h>

namespace nw {
namespace graph {

//****************************************************************************
template <typename Graph, typename score_t = float, typename accum_t = size_t>
std::vector<score_t> betweenness_brandes(const Graph& A) {
  using vertex_id_type = typename Graph::vertex_id_type;

  size_t               n_vtx = A.size();
  std::vector<score_t> centrality(n_vtx, 0);
  auto                 G = A.begin();

  std::stack<vertex_id_type> S;
  std::queue<vertex_id_type> Q;
  std::vector<accum_t>       path_counts(n_vtx);
  std::vector<score_t>       d(n_vtx);

  for (auto outer = G; outer != A.end(); ++outer) {
    auto s = outer - G;

    path_counts.assign(n_vtx, 0);
    d.assign(n_vtx, -1);
    std::vector<std::list<size_t>> P(n_vtx);

    path_counts[s] = 1;
    d[s]           = 0;
    Q.push(s);

    while (!Q.empty()) {
      auto v = Q.front();
      Q.pop();
      S.push(v);
      for (auto inner = G[v].begin(); inner != G[v].end(); ++inner) {
        auto w = std::get<0>(*inner);
        if (d[w] < 0) {
          Q.push(w);
          d[w] = d[v] + 1;
        }
        if (d[w] == (d[v] + 1)) {
          path_counts[w] += path_counts[v];
          P[w].push_back(v);
        }
      }
    }

    std::vector<score_t> delta(n_vtx, 0);
    while (!S.empty()) {
      auto w = S.top();
      S.pop();
      for (auto it = P[w].begin(); it != P[w].end(); ++it) {
        delta[*it] += static_cast<score_t>(path_counts[*it]) / static_cast<score_t>(path_counts[w]) * (1 + delta[w]);
      }
      if (w != s) {
        centrality[w] += delta[w];
      }
    }
  }
  std::vector<score_t> final(n_vtx);
  score_t              largest = *std::max_element(centrality.begin(), centrality.end());
  std::transform(centrality.begin(), centrality.end(), final.begin(), [&](auto& val) { return val / largest; });
  return final;
}

template <class score_t, class accum_t, class Graph, class OuterExecutionPolicy = std::execution::parallel_unsequenced_policy,
          class InnerExecutionPolicy = std::execution::parallel_unsequenced_policy>
auto bc2_v5(const Graph& graph, const std::vector<typename Graph::vertex_id_type>& sources, int threads,
            OuterExecutionPolicy&& outer_policy = {}, InnerExecutionPolicy&& inner_policy = {}) {
  using vertex_id_type = typename Graph::vertex_id_type;

  vertex_id_type       N     = num_vertices(graph);
  size_t               M     = graph.to_be_indexed_.size();
  auto&&               edges = std::get<0>(*(graph[0]).begin());
  std::vector<score_t> bc(N);

  const vertex_id_type num_bins = nw::graph::pow2(nw::graph::ceil_log2(threads));
  const vertex_id_type bin_mask = num_bins - 1;

  std::vector<std::future<void>> futures(sources.size());
  for (size_t s_idx = 0; s_idx < sources.size(); ++s_idx) {
    futures[s_idx] = std::async(
        std::launch::async,
        [&](vertex_id_type root) {
          std::vector<vertex_id_type> levels(N);
          nw::graph::AtomicBitVector  succ(M);

          // Initialize the levels to infinity.
          std::fill(outer_policy, levels.begin(), levels.end(), std::numeric_limits<vertex_id_type>::max());

          std::vector<accum_t>                                             path_counts(N);
          std::vector<tbb::concurrent_vector<vertex_id_type>>              q1(num_bins);
          std::vector<tbb::concurrent_vector<vertex_id_type>>              q2(num_bins);
          std::vector<std::vector<tbb::concurrent_vector<vertex_id_type>>> retired;

          vertex_id_type lvl = 0;

          path_counts[root] = 1;
          q1[0].push_back(root);
          levels[root] = lvl++;

          bool done = false;
          while (!done) {
            std::for_each(outer_policy, q1.begin(), q1.end(), [&](auto&& q) {
              std::for_each(inner_policy, q.begin(), q.end(), [&](auto&& u) {
                for (auto&& [v] : graph[u]) {
                  auto&& infinity = std::numeric_limits<vertex_id_type>::max();
                  auto&& lvl_v    = nw::graph::acquire(levels[v]);

                  // If this is our first encounter with this node, or
                  // it's on the right level, then propagate the counts
                  // from u to v, and mark the edge from u to v as used.
                  if (lvl_v == infinity || lvl_v == lvl) {
                    nw::graph::fetch_add(path_counts[v], nw::graph::acquire(path_counts[u]));
                    succ.atomic_set(&v - &edges);    // edge(w,v) : P[w][v]
                  }

                  // We need to add v to the frontier exactly once the
                  // first time we encounter it, so we race to set its
                  // level and if we win that race we can be the one to
                  // add it.
                  if (lvl_v == infinity && nw::graph::cas(levels[v], infinity, lvl)) {
                    q2[u & bin_mask].push_back(v);
                  }
                }
              });
            });

            done = true;
            for (size_t i = 0; i < num_bins; ++i) {
              if (q2[i].size() != 0) {
                done = false;
                break;
              }
            }

            retired.emplace_back(num_bins);
            std::swap(q1, retired.back());
            std::swap(q1, q2);

            ++lvl;
          }

          std::vector<score_t> deltas(N);

          std::for_each(retired.rbegin(), retired.rend(), [&](auto&& vvv) {
            std::for_each(outer_policy, vvv.begin(), vvv.end(), [&](auto&& vv) {
              std::for_each(inner_policy, vv.begin(), vv.end(), [&](auto&& u) {
                score_t delta = 0;
                for (auto&& [v] : graph[u]) {
                  if (succ.get(&v - &edges)) {
                    delta += path_counts[u] / path_counts[v] * (1.0f + deltas[v]);
                  }
                }
                nw::graph::fetch_add(bc[u], deltas[u] = delta);
              });
            });
          });
        },
        sources[s_idx]);
  }

  for (auto&& f : futures) {
    f.wait();
  }

  auto max = std::reduce(outer_policy, bc.begin(), bc.end(), 0.0f, nw::graph::max{});
  std::for_each(outer_policy, bc.begin(), bc.end(), [&](auto&& j) { j /= max; });

  return bc;
}

}    // namespace graph
}    // namespace nw
#endif    // BETWEENNESS_CENTRALITY_HPP
