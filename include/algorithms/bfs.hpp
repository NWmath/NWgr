// This file is part of the Graph Standard Library (aka BGL17 aka NWGraph)
//
// (c) Pacific Northwest National Laboratory 2020
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0
// International License https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#ifndef NW_GRAPH_BFS_HPP
#define NW_GRAPH_BFS_HPP

#include "util/types.hpp"
#include "containers/compressed.hpp"
#include "util/AtomicBitVector.hpp"
#include "util/atomic.hpp"
#include "util/parallel_for.hpp"
#include <queue>

#include <tbb/concurrent_queue.h>
#include <tbb/concurrent_vector.h>
#include <tbb/parallel_for_each.h>

#if defined(CL_SYCL_LANGUAGE_VERSION)
#include <dpstd/iterators.h>
namespace nw::graph {
template <class T>
using counting_iterator = dpstd::counting_iterator<T>;
}
#else
#include <tbb/iterators.h>
namespace nw::graph {
template <class T>
using counting_iterator = tbb::counting_iterator<T>;
}
#endif

namespace nw {
namespace graph {
template<typename Graph, typename GraphT>
bool BFSVerifier(Graph& g, GraphT& g_t, vertex_id_t source, std::vector<vertex_id_t>& parent) {
  std::vector<vertex_id_t> depth(g.max() + 1, std::numeric_limits<vertex_id_t>::max());
  depth[source] = 0;
  std::vector<vertex_id_t> to_visit;
  to_visit.reserve(g.max() + 1);
  to_visit.push_back(source);
  auto out_neigh = g.begin();
  auto in_neigh  = g_t.begin();
  for (auto it = to_visit.begin(); it != to_visit.end(); it++) {
    vertex_id_t u = *it;
    for (auto edge : out_neigh[u]) {
      vertex_id_t v = std::get<0>(edge);
      if (depth[v] == std::numeric_limits<vertex_id_t>::max()) {
        depth[v] = depth[u] + 1;
        to_visit.push_back(v);
      }
    }
  }
  for (vertex_id_t u = 0; u < g.max() + 1; ++u) {
    if ((depth[u] != std::numeric_limits<vertex_id_t>::max()) && (parent[u] != std::numeric_limits<vertex_id_t>::max())) {
      if (u == source) {
        if (!((parent[u] == u) && (depth[u] == 0))) {
          std::cout << "Source wrong " << u << " " << parent[u] << " " << depth[u] << std::endl;
          return false;
        }
        continue;
      }
      bool parent_found = false;
      for (auto edge : in_neigh[u]) {
        vertex_id_t v = std::get<0>(edge);
        if (v == parent[u]) {
          //if(it != out_neigh[v].end()) {
          if (depth[v] != depth[u] - 1) {
            std::cout << "Wrong depths for " << u << " & " << v << std::endl;
            return false;
          }
          parent_found = true;
          break;
        }
      }
      if (!parent_found) {
        std::cout << "Couldn't find edge from " << parent[u] << " to " << u << std::endl;
        return false;
      }
    } else if (depth[u] != parent[u]) {
      std::cout << "Reachability mismatch " << u << " " << depth[u] << " " << parent[u] << std::endl;
      return false;
    }
  }
  return true;
}

template <typename Graph>
auto bfs_v0(Graph& graph, vertex_id_t root) {

  std::deque<vertex_id_t>  q1, q2;
  std::vector<vertex_id_t> level(graph.max() + 1, std::numeric_limits<vertex_id_t>::max());
  std::vector<vertex_id_t> parents(graph.max() + 1, std::numeric_limits<vertex_id_t>::max());
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
class _concurrent_queue : public tbb::concurrent_queue<T> {
  using base = tbb::concurrent_queue<T>;

public:
  auto internal_swap(_concurrent_queue& src) { base::internal_swap(src); }
};

template <typename Graph>
auto bfs_v4(Graph& graph, vertex_id_t root) {

  _concurrent_queue<vertex_id_t> q1, q2;
  std::vector                    level(graph.max() + 1, std::numeric_limits<vertex_id_t>::max());
  std::vector                    parents(graph.max() + 1, std::numeric_limits<vertex_id_t>::max());
  size_t                         lvl = 0;

  q1.push(root);
  level[root] = lvl++;

  auto g = graph.begin();

  while (!q1.empty()) {

    std::for_each(std::execution::par_unseq, q1.unsafe_begin(), q1.unsafe_end(), [&](vertex_id_t u) {
      std::for_each(g[u].begin(), g[u].end(), [&](auto&& x) {
        vertex_id_t v = std::get<0>(x);
        if (level[v] == std::numeric_limits<vertex_id_t>::max()) {
          q2.push(v);
          level[v]   = lvl;
          parents[v] = u;
        }
      });
    });
    q1.internal_swap(q2);
    q2.clear();

    ++lvl;
  }
  return parents;
}

template <typename Graph>
auto bfs_v6(Graph& graph, vertex_id_t root) {

  tbb::concurrent_vector<vertex_id_t> q1, q2;
  std::vector<vertex_id_t>            level(graph.max() + 1, std::numeric_limits<vertex_id_t>::max());
  std::vector<vertex_id_t>            parents(graph.max() + 1, std::numeric_limits<vertex_id_t>::max());
  size_t                              lvl = 0;

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

template <typename Graph>
auto bfs_v7(Graph& graph, vertex_id_t root) {

  tbb::concurrent_vector<vertex_id_t>   q1, q2;
  std::vector<std::atomic<vertex_id_t>> level(graph.max() + 1);
  for (size_t i = 0; i < graph.size(); ++i) {
    level[i] = std::numeric_limits<vertex_id_t>::max();
  }
  std::vector parents(graph.max() + 1, std::numeric_limits<vertex_id_t>::max());
  size_t      lvl = 0;

  q1.push_back(root);
  level[root]   = lvl++;
  parents[root] = root;

  auto g = graph.begin();

  while (!q1.empty()) {
    std::for_each(std::execution::par_unseq, q1.begin(), q1.end(), [&](vertex_id_t u) {
      std::for_each(g[u].begin(), g[u].end(), [&](auto&& x) {
        vertex_id_t v       = std::get<0>(x);
        vertex_id_t old_lvl = level[v];
        vertex_id_t new_lvl = lvl;
        if (new_lvl < old_lvl) {
          bool changed = true;
          while (!level[v].compare_exchange_strong(old_lvl, new_lvl)) {
            if (old_lvl <= new_lvl) {
              changed = false;
              break;
            }
          }
          if (changed) {
            q2.push_back(v);
            parents[v] = u;
          }
        }
      });
    });
    std::swap(q1, q2);
    q2.clear();

    ++lvl;
  }
  return parents;
}

template <typename Graph>
auto bfs_v8(Graph& graph, vertex_id_t root) {

  _concurrent_queue<vertex_id_t> q1, q2;
  std::vector                    level(graph.max() + 1, std::numeric_limits<vertex_id_t>::max());
  std::vector                    parents(graph.max() + 1, std::numeric_limits<vertex_id_t>::max());
  size_t                         lvl = 0;

  q1.push(root);
  level[root]   = lvl++;
  parents[root] = root;

  auto g = graph.begin();

  while (!q1.empty()) {

    std::for_each(std::execution::par_unseq, q1.unsafe_begin(), q1.unsafe_end(), [&](vertex_id_t u) {
      std::for_each(std::execution::par_unseq, g[u].begin(), g[u].end(), [&](auto&& x) {
        vertex_id_t v = std::get<0>(x);
        if (level[v] == std::numeric_limits<vertex_id_t>::max()) {
          q2.push(v);
          level[v]   = lvl;
          parents[v] = u;
        }
      });
    });
    q1.internal_swap(q2);
    q2.clear();

    ++lvl;
  }
  return parents;
}

template <typename Graph>
auto bfs_v9(Graph& graph, vertex_id_t root) {

  const size_t                                     num_bins = 32;
  const size_t                                     bin_mask = 0x1F;
  std::vector<tbb::concurrent_vector<vertex_id_t>> q1(num_bins), q2(num_bins);
  std::vector                                      level(graph.max() + 1, std::numeric_limits<vertex_id_t>::max());
  std::vector                                      parents(graph.max() + 1, std::numeric_limits<vertex_id_t>::max());
  size_t                                           lvl = 0;
  parents[root]                                        = root;

  q1[0].push_back(root);
  level[root]   = lvl++;
  parents[root] = root;

  auto g = graph.begin();

  bool done = false;
  while (!done) {
    std::for_each(std::execution::par_unseq, q1.begin(), q1.end(), [&](auto& q) {
      std::for_each(std::execution::par_unseq, q.begin(), q.end(), [&](vertex_id_t u) {
        tbb::parallel_for(g[u], [&](auto&& gu) {
          std::for_each(gu.begin(), gu.end(), [&](auto&& x) {
            vertex_id_t v = std::get<0>(x);
            if (level[v] == std::numeric_limits<vertex_id_t>::max()) {
              q2[u & bin_mask].push_back(v);
              level[v]   = lvl;
              parents[v] = u;
            }
          });
        });
      });
    });

    done = true;
    for (size_t i = 0; i < num_bins; ++i) {
      if (q2[i].size() != 0) {
        done = false;
        break;
      }
    }

    std::swap(q1, q2);

    for (size_t i = 0; i < num_bins; ++i) {
      q2[i].clear();
    }

    ++lvl;
  }
  return parents;
}

template <class Graph>
[[gnu::noinline]] auto bfs_top_down(Graph&& graph, vertex_id_t root) {
  constexpr const std::size_t                      num_bins = 32;
  const std::size_t                                N        = graph.max() + 1;
  std::vector<tbb::concurrent_vector<vertex_id_t>> q1(num_bins);
  std::vector<tbb::concurrent_vector<vertex_id_t>> q2(num_bins);
  std::vector<vertex_id_t>                         parents(N);

  std::fill(std::execution::par_unseq, parents.begin(), parents.end(), null_vertex);

  q1[0].push_back(root);
  parents[root] = root;

  bool done = false;
  while (!done) {
    std::for_each(std::execution::par_unseq, q1.begin(), q1.end(), [&](auto&& q) {
      std::for_each(std::execution::par_unseq, q.begin(), q.end(), [&](auto&& u) {
        nw::graph::parallel_for(graph[u], [&](auto&& v) {
          if (nw::graph::relaxed(parents[v]) == null_vertex) {
            nw::graph::relaxed(parents[v], u);
            q2[u % num_bins].push_back(v);
          }
        });
      });
    });

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

template <class Graph>
[[gnu::noinline]] auto bfs_top_down_bitmap(Graph&& graph, vertex_id_t root) {
  constexpr const std::size_t                      num_bins = 32;
  const std::size_t                                N        = graph.max() + 1;
  std::vector<tbb::concurrent_vector<vertex_id_t>> q1(num_bins);
  std::vector<tbb::concurrent_vector<vertex_id_t>> q2(num_bins);
  std::vector<vertex_id_t>                         parents(N);
  nw::graph::AtomicBitVector                           visited(N);

  std::fill(std::execution::par_unseq, parents.begin(), parents.end(), null_vertex);

  q1[0].push_back(root);
  parents[root] = root;
  visited.set(root);

  bool done = false;
  while (!done) {
    std::for_each(std::execution::par_unseq, q1.begin(), q1.end(), [&](auto&& q) {
      std::for_each(std::execution::par_unseq, q.begin(), q.end(), [&](auto&& u) {
        nw::graph::parallel_for(graph[u], [&](auto&& v) {
          if (visited.atomic_get(v) == 0 && visited.atomic_set(v) == 0) {
            parents[v] = u;
            q2[u % num_bins].push_back(v);
          }
        });
      });
    });

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

template <class Graph, class Transpose>
[[gnu::noinline]] auto bfs_bottom_up(Graph&& g, Transpose&& gx, vertex_id_t root) {
  const std::size_t      N = gx.max() + 1;
  nw::graph::AtomicBitVector frontier(N);
  nw::graph::AtomicBitVector next(N);

  std::vector<vertex_id_t> parents(N);
  std::fill(std::execution::par_unseq, parents.begin(), parents.end(), null_vertex);

  parents[root] = root;
  next.set(root);
  for (vertex_id_t n = 1; n != 0; n = tbb::parallel_reduce(
                                      tbb::blocked_range(0ul, N), 0,
                                      [&](auto&&range, auto n) {
                                        for (auto &&v = range.begin(), e = range.end(); v != e; ++v) {
                                          if (parents[v] == null_vertex) {
                                            for (auto&& [u] : gx[v]) {
                                              if (frontier.get(u)) {
                                                next.atomic_set(v);
                                                parents[v] = u;
                                                ++n;
                                                break;
                                              }
                                            }
                                          }
                                        }
                                        return n;
                                      },
                                      std::plus{})) {
    std::swap(frontier, next);
    next.clear();
  }
  return parents;
}

template <typename OutGraph, typename InGraph>
[[gnu::noinline]] auto bfs_v11(OutGraph& out_graph, InGraph& in_graph, vertex_id_t root, int num_bins = 32, int alpha = 15,
                               int beta = 18) {
  const std::size_t                                n = nw::graph::pow2(nw::graph::ceil_log2(num_bins));
  const std::size_t                                N = out_graph.max() + 1;
  const std::size_t                                M = out_graph.to_be_indexed_.size();
  std::vector<tbb::concurrent_vector<vertex_id_t>> q1(n), q2(n);

  nw::graph::AtomicBitVector   visited(N);
  std::vector<vertex_id_t> parents(N);

  std::fill(std::execution::par_unseq, parents.begin(), parents.end(), null_vertex);

  std::uint64_t edges_to_check = M;
  std::uint64_t scout_count    = out_graph[root].size();

  visited.atomic_set(root);
  parents[root] = root;
  q1[root % n].push_back(root);

  bool done = false;
  while (!done) {
    if (scout_count > edges_to_check / alpha) {
      nw::graph::AtomicBitVector front(N, false);
      nw::graph::AtomicBitVector curr(N);
      std::size_t            awake_count = 0;

      // Initialize the frontier bitmap from the frontier queues, and count the
      // number of non-zeros.
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
            [&](auto&& range, auto n) {
              for (auto &&v = range.begin(), e = range.end(); v != e; ++v) {
                if (visited.get(v) == 0) {
                  for (auto&& [u] : in_graph[v]) {
                    if (front.get(u) && visited.atomic_set(v) == 0) {
                      curr.atomic_set(v);
                      parents[v] = u;
                      ++n;
                      break;
                    }
                  }
                }
              }
              return n;
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
                        if (visited.atomic_get(v) == 0 && visited.atomic_set(v) == 0) {
                          q2[u % n].push_back(v);
                          parents[v] = u;
                          return out_graph[v].size();
                        }
                        return 0ul;
                      },
                      std::plus{}, 0ul);
                },
                std::plus{}, 0ul);
          },
          std::plus{}, 0ul);
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
