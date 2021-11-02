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
//     Xu Tony Liu	
//

#ifndef NW_GRAPH_BFS_HPP
#define NW_GRAPH_BFS_HPP

#include "nwgraph/containers/compressed.hpp"
#include "nwgraph/graph_traits.hpp"
#include "nwgraph/util/AtomicBitVector.hpp"
#include "nwgraph/util/atomic.hpp"
#include "nwgraph/util/parallel_for.hpp"
#include "nwgraph/adaptors/neighbor_range.hpp"
#include "nwgraph/adaptors/cyclic_range_adapter.hpp"
#include "nwgraph/adaptors/vertex_range.hpp"
#include <queue>

#include <tbb/concurrent_queue.h>
#include <tbb/concurrent_vector.h>
#include <tbb/parallel_for_each.h>


namespace nw {
namespace graph {

template <typename Graph>
auto bfs_v0(const Graph& graph, typename graph_traits<Graph>::vertex_id_type root) {
  using vertex_id_type = typename graph_traits<Graph>::vertex_id_type;

  std::deque<vertex_id_type>  q1, q2;
  std::vector<vertex_id_type> level(num_vertices(graph), std::numeric_limits<vertex_id_type>::max());
  std::vector<vertex_id_type> parents(num_vertices(graph), std::numeric_limits<vertex_id_type>::max());
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


template <typename OutGraph, typename InGraph>
[[gnu::noinline]] auto bfs_v11(const OutGraph& out_graph, const InGraph& in_graph, vertex_id_t<OutGraph> root, int num_bins = 32,
                               int alpha = 15, int beta = 18) {

  using vertex_id_type = vertex_id_t<OutGraph>;

  const std::size_t                                   n = nw::graph::pow2(nw::graph::ceil_log2(num_bins));
  const std::size_t                                   N = num_vertices(out_graph);
  const std::size_t                                   M = out_graph.to_be_indexed_.size();
  std::vector<tbb::concurrent_vector<vertex_id_type>> q1(n), q2(n);

  std::vector<vertex_id_type> parents(N);
  nw::graph::AtomicBitVector front(N, false);
  nw::graph::AtomicBitVector curr(N);

  constexpr const auto null_vertex = null_vertex_v<vertex_id_type>();
  std::fill(std::execution::par_unseq, parents.begin(), parents.end(), null_vertex);

  std::uint64_t edges_to_check = M;
  std::uint64_t scout_count    = out_graph[root].size();

  parents[root] = root;
  q1[root % n].push_back(root);

  bool done = false;
  while (!done) {
    if (scout_count > edges_to_check / alpha) {
      std::size_t awake_count = 0;
      // Initialize the frontier bitmap from the frontier queues, and count the
      // number of non-zeros.
      /*
      std::size_t awake_count = nw::graph::parallel_for(
          tbb::blocked_range(0ul, q1.size()),
          [&](auto&& i) {
            auto&& q = q1[i];
            std::for_each(q.begin(), q.end(), [&](auto&& u) { curr.atomic_set(u); });
            return q.size();
          }, std::plus{}, 0ul);
      */
      std::for_each(std::execution::par_unseq, q1.begin(), q1.end(), [&](auto&& q) {
        nw::graph::fetch_add(awake_count, q.size());
        std::for_each(std::execution::par_unseq, q.begin(), q.end(), [&](auto&& u) { curr.atomic_set(u); });
      });

      std::size_t old_awake_count = 0;
      do {
        old_awake_count = awake_count;
        std::swap(front, curr);
        curr.clear();

        awake_count = tbb::parallel_reduce(
            tbb::blocked_range(0ul, N), 0ul,
            [&](auto&& range, auto count) {
              for (auto&& u = range.begin(), e = range.end(); u != e; ++u) {
                if (null_vertex == parents[u]) {
                  for (auto&& [v] : in_graph[u]) {
                    if (front.get(v)) {
                      curr.atomic_set(u);
                      parents[u] = v;
                      ++count;
                      break;
                    }
                  }
                }
              }
              return count;
            },
            std::plus{});
      } while ((awake_count >= old_awake_count) || (awake_count > N / beta));

      if (awake_count == 0) {
        return parents;
      }

      tbb::parallel_for(curr.non_zeros(nw::graph::pow2(15)), [&](auto&& range) {
        for (auto &&i = range.begin(), e = range.end(); i != e; ++i) {
          q2[*i % n].push_back(*i);
        }
      });

      scout_count = 1;
    } else {
      edges_to_check -= scout_count;
      /*
      scout_count = nw::graph::parallel_for(
          tbb::blocked_range(0ul, q1.size()),
          [&](auto&& i) {
            auto&& q = q1[i];
            return nw::graph::parallel_for(
                tbb::blocked_range(0ul, q.size()),
                [&](auto&& i) {
                  auto&& u = q[i];
                  return nw::graph::parallel_for(
                      out_graph[u],
                      [&](auto&& v) {
                        auto curr_val = parents[v];
                        if (null_vertex == curr_val) {
                          if (nw::graph::cas(parents[v], curr_val, u)) {
                            q2[u % n].push_back(v);
                            return out_graph[v].size();
                          }
                        }
                        return 0ul;
                      },
                      std::plus{}, 0ul);
                },
                std::plus{}, 0ul);
          },
          std::plus{}, 0ul);
          */
        scout_count = nw::graph::parallel_for(
          tbb::blocked_range(0ul, q1.size()),
          [&](auto&& i) {
            auto&& q = q1[i];
            return nw::graph::parallel_for(
                tbb::blocked_range(0ul, q.size()),
                [&](auto&& i) {
                  size_t count = 0;
                  auto&& u = q[i];
                  for (auto&& [v] : out_graph[u]) {
                    auto curr_val = parents[v];
                    if (null_vertex == curr_val) {
                      if (nw::graph::cas(parents[v], curr_val, u)) {
                        q2[u % n].push_back(v);
                        count += out_graph[v].size();
                      }
                    }
                  }
                  return count;
                }, std::plus{}, 0ul);
        }, std::plus{}, 0ul);
    }

    done = true;
    for (auto&& q : q2) {
      if (q.size() != 0) {
        done = false;
        break;
      }
    }
    std::swap(q1, q2);
    for (auto&& q : q2) {
      q.clear();
    }
    
  }

  return parents;
}


}    // namespace graph
}    // namespace nw

#endif    // NW_GRAPH_BFS_HPP
