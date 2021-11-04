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

#ifndef CONNECTED_COMPONENT_EXPERIMENTAL_HPP
#define CONNECTED_COMPONENT_EXPERIMENTAL_HPP

#include "nwgraph/adaptors/bfs_edge_range.hpp"
#include "nwgraph/adaptors/edge_range.hpp"
#include "nwgraph/adaptors/vertex_range.hpp"
#include <atomic>
#include <iostream>
#include <random>
#include <unordered_map>

#if defined(CL_SYCL_LANGUAGE_VERSION)
#include <dpstd/algorithm>
#include <dpstd/execution>
#else
#include <algorithm>
#include <execution>
#endif

namespace nw {
namespace graph {

template <typename T>
inline bool compare_and_swap(T& x, T old_val, T new_val) {
  return __sync_bool_compare_and_swap(&x, *(&old_val), *(&new_val));
}
template <typename T>
struct atomwrapper {
  std::atomic<T> _a;

  atomwrapper() : _a() {}

  atomwrapper(const std::atomic<T>& a) : _a(a.load()) {}

  atomwrapper(const atomwrapper& other) : _a(other._a.load()) {}

  atomwrapper& operator=(const atomwrapper& other) {
    _a.store(other._a.load());
    return *this;
  }
};

// BFS-based connected component algorithm
template <typename Graph, typename T>
void compute_connected_components(Graph A, std::vector<T>& component_ids) {
  size_t         N                        = A.size();
  std::atomic<T> global_component_counter = -1;
  std::for_each(std::execution::par_unseq, counting_iterator<T>(0), counting_iterator<T>(N), [&](auto vtx) {
    if (std::numeric_limits<T>::max() == component_ids[vtx]) {
      global_component_counter++;
      component_ids[vtx] = global_component_counter.load();
      //      std::cout << "--------vertex--------------" << vtx << "
      //      component_ids[vtx]: " << component_ids[vtx] << std::endl;
      bfs_edge_range3 ranges(A, vtx);
      for (auto ite = ranges.begin(); ite != ranges.end(); ++ite) {
        // auto u = std::get<0>(*ite);
        auto v           = std::get<1>(*ite);
        component_ids[v] = global_component_counter.load();
      }
    }
  });
}

template <typename T>
void hook(T u, T v, std::vector<T>& comp) {
  T p1 = comp[u];
  T p2 = comp[v];
  T high, low;
  while (p1 != p2) {
    high              = std::max(p1, p2);
    low               = p1 + p2 - high;
    volatile T p_high = comp[high];
    if (p_high == low || compare_and_swap(comp[high], high, low)) break;
    p1 = comp[comp[high]];
    p2 = comp[low];
  }    // while
}

template <typename T>
void compress(std::vector<T>& comp) {
  std::for_each(std::execution::par_unseq, counting_iterator<T>(0), counting_iterator<T>(comp.size()), [&](auto n) {
    while (comp[n] != comp[comp[n]])
      comp[n] = comp[comp[n]];
  });
}

template <typename T>
T findDominantComponentID(const std::vector<T>& comp, size_t nsamples = 1024) {
  if (0 == comp.size()) return 0;
  std::unordered_map<T, size_t>                              sample_counts(32);
  typedef typename std::unordered_map<T, size_t>::value_type kvp_type;
  std::mt19937                                               gen;
  std::uniform_int_distribution<T>                           distribution(0, comp.size() - 1);
  for (size_t i = 0; i < nsamples; i++) {
    T n = distribution(gen);
    ++sample_counts[comp[n]];
  }
  auto dominant =
      std::max_element(sample_counts.begin(), sample_counts.end(), [](const kvp_type& a, const kvp_type& b) { return a.second < b.second; });
  /*
    float frac_of_graph = static_cast<float>(dominant->second) / nsamples;
    std::cout
      << "Skipping largest intermediate component (ID: " << dominant->first
      << ", approx. " << static_cast<int>(frac_of_graph * 100)
      << "% of the graph)" << std::endl;
  */
  return dominant->first;
}

template <typename Graph, typename T>
void push(const Graph& g, const T u, std::vector<T>& comp) {
  for (auto j = g.begin()[u].begin(); j != g.begin()[u].end(); ++j) {
    auto v = std::get<0>(*j);
    hook(u, v, comp);
  }
}

template <typename Graph, typename T>
void link(const Graph& g, const T u, std::vector<T>& comp, const size_t neighbor_bound) {
  size_t i = 0;
  for (auto j = g.begin()[u].begin(); j != g.begin()[u].end() && i < neighbor_bound; ++j, ++i) {
    auto v = std::get<0>(*j);
    hook(u, v, comp);
  }
}

// fetch the smallest comp_id among u's neighbors
template <typename Graph, typename T>
bool pull(const Graph& g, const T u, std::vector<T>& comp) {
  T min_compid = comp[u];
  T v;
  for (auto j = g.begin()[u].begin(); j != g.begin()[u].end(); ++j) {
    v          = std::get<0>(*j);
    min_compid = std::min(min_compid, comp[v]);
  }
  bool change = false;
  T    p1     = comp[u];
  T    p2     = min_compid;
  T    high = comp[u], low = min_compid;
  while (p1 != p2) {
    high              = std::max(p1, p2);
    low               = p1 + p2 - high;
    volatile T p_high = comp[high];
    if (p_high == low || compare_and_swap(comp[high], high, low)) {
      change = true;
      break;
    }
    p1 = comp[comp[high]];
    p2 = comp[low];
  }    // while
  return change;
}

template <typename Graph, typename T = vertex_id_t<Graph>>
std::vector<T> compute_connected_components_v1(const Graph& g) {
  size_t N = g.size();
  // std::vector<atomwrapper<T>> comp(N,
  // atomwrapper<T>(-1));
  std::vector<atomwrapper<T>> comp(N);

  std::for_each(std::execution::par_unseq, counting_iterator<T>(0), counting_iterator<T>(N), [&](auto n) { comp[n]._a.store(n); });

  bool change = true;
  for (size_t num_iter = 0; num_iter < 2; ++num_iter) {
    if (false == change) {
      std::cout << "Shiloach-Vishkin took " << num_iter << " iterations" << std::endl;
      break;
    }
    change = false;
    /*
     * TODO bfs edge based parallel iterating over graph
      bfs_edge_range3 range(g, 0);
      auto first = range.begin();
      auto last = range.end();
      std::for_each(
    std::execution::par_unseq,
      first, last, [&](auto&& ite) {
        auto u = std::get<0>(*ite);
        auto v = std::get<1>(*ite);

          if (v != comp[v]._a.load()) return;
          auto p1 = comp[u]._a.load();
          auto p2 = comp[v]._a.load();
          while (p1 != p2) {
            auto high = std::max(p1, p2);
            auto low = p1 + p2 - high;
            auto p_high = comp[high]._a.load();
            if (p_high == low || comp[high]._a.compare_exchange_weak(p_high,
    comp[low]._a)) { change = true; break;
            }
            high = comp[high]._a.load();
            p1 = comp[high]._a.load();
            p2 = comp[low]._a.load();
          }
        });
    */
    std::for_each(std::execution::par_unseq, counting_iterator<T>(0), counting_iterator<T>(N), [&](auto u) {
      T v;
      for (auto j = g.begin()[u].begin(); j != g.begin()[u].end(); ++j) {
        v = std::get<0>(*j);
        //      if (v != comp[v]._a.load()) continue;
        auto p1 = comp[u]._a.load();
        auto p2 = comp[v]._a.load();
        while (p1 != p2) {
          auto high   = std::max(p1, p2);
          auto low    = p1 + p2 - high;
          auto p_high = comp[high]._a.load();
          if (p_high == low || comp[high]._a.compare_exchange_weak(p_high, comp[low]._a)) {
            change = true;
            break;
          }
          high = comp[high]._a.load();
          p1   = comp[high]._a.load();
          p2   = comp[low]._a.load();
        }    // while
      }
    });
  }
  std::vector<T> res(N);
  std::for_each(std::execution::par_unseq, counting_iterator<T>(0), counting_iterator<T>(N), [&](auto n) {
    auto m = n;
    //    while (comp[m]._a.load() != m)  m = comp[m]._a.load();
    while (comp[m]._a.load() != comp[comp[m]._a.load()]._a.load()) {
      comp[m]._a.store(comp[comp[m]._a.load()]._a.load());
      m = comp[m]._a.load();
    }
    res[n] = comp[n]._a.load();
    // std::cout << n <<":" <<  res[n] << std::endl;
    /*
          atomwrapper<T> m(std::atomic<T>(n));
          while ( comp[m._a.load()] != comp[comp[m._a.load()]._a.load()]) {
            m._a.store(comp[m._a.load()]);
          }
          res[n] = comp[m._a.load()].a_.load();
        */
  });
  // compress(comp);
  return res;
}    // compute_connected_components_v1

template <typename Graph, typename T = vertex_id_t<Graph>>
std::vector<T> compute_connected_components_v2(const Graph& g) {
  size_t         N = g.size();
  std::vector<T> comp(g.size());
  std::for_each(std::execution::par_unseq, counting_iterator<T>(0), counting_iterator<T>(g.size()), [&](auto n) { comp[n] = n; });

  bool change = true;

  for (size_t num_iter = 0; num_iter < 1; ++num_iter) {
    if (false == change) {
      std::cout << "PULL-cc took " << num_iter << " iterations" << std::endl;
      break;
    }
    change = false;

    std::for_each(std::execution::par_unseq, counting_iterator<T>(0), counting_iterator<T>(N), [&](auto u) { change = pull(g, u, comp); });
    std::for_each(std::execution::par_unseq, counting_iterator<T>(0), counting_iterator<T>(N), [&](auto u) { push(g, u, comp); });

    compress(comp);
  }
  return comp;
}    // compute_connected_components_v2

template <typename Graph, typename T>
std::vector<T> ccv1(const Graph& g) {
  std::vector<T> comp(g.size());
  std::for_each(std::execution::par_unseq, counting_iterator<T>(0), counting_iterator<T>(g.size()), [&](auto n) { comp[n] = n; });

  std::for_each(std::execution::par_unseq, counting_iterator<T>(0), counting_iterator<T>(g.size()), [&](auto u) { push(g, u, comp); });
  compress(comp);
  return comp;
}

template <typename Graph, typename Graph2, typename T = vertex_id_t<Graph>>
std::vector<T> Afforest(const Graph& g, Graph2& t_graph, size_t neighbor_bound = 2) {
  std::vector<T> comp(g.size());
  // set component id of vertex v to v
  std::for_each(std::execution::par_unseq, counting_iterator<T>(0), counting_iterator<T>(g.size()), [&](auto n) { comp[n] = n; });
  // approximate the dominant component by linking certain neighbors of each
  // vertex v (a sparse subgraph)
  std::for_each(std::execution::par_unseq, counting_iterator<T>(0), counting_iterator<T>(g.size()),
                [&](auto u) { link(g, u, comp, neighbor_bound); });
  compress(comp);
  // Sample certain vertices to find dominant component id
  T dominant_c = findDominantComponentID(comp);
  // link the rest vertices outside of dominant component
  std::for_each(std::execution::par_unseq, counting_iterator<T>(0), counting_iterator<T>(g.size()), [&](auto u) {
    if (dominant_c != comp[u]) {
      push(g, u, comp);
      if (t_graph.size() != 0) {
        push(t_graph, u, comp);
      }
    }
  });
  compress(comp);

  return comp;
}

template <typename Graph, typename T = vertex_id_t<Graph>>
std::vector<T> ccv5(const Graph& g) {
  size_t         N = g.size();
  std::vector<T> comp(g.size());
  std::for_each(std::execution::par_unseq, counting_iterator<T>(0), counting_iterator<T>(g.size()), [&](auto n) { comp[n] = n; });

  std::for_each(std::execution::par_unseq, counting_iterator<T>(0), counting_iterator<T>(N), [&](auto u) { pull(g, u, comp); });
  T dominant_c = findDominantComponentID(comp);
  std::for_each(std::execution::par_unseq, counting_iterator<T>(0), counting_iterator<T>(N), [&](auto u) {
    if (dominant_c != comp[u]) push(g, u, comp);
  });

  compress(comp);
  return comp;
}

template <typename Graph, typename T= vertex_id_t<Graph>>
auto sv_v6(const Graph& g) {
  std::vector<T> comp(g.size());

  std::for_each(std::execution::par_unseq, counting_iterator<T>(0), counting_iterator<T>(g.size()), [&](auto n) { comp[n] = n; });

  bool change = true;

  auto G = g.begin();

  for (size_t num_iter = 0; num_iter < g.size(); ++num_iter) {
    if (false == change) {
      // std::cout << "Shiloach-Vishkin took " << num_iter << " iterations" <<
      // std::endl;
      break;
    }
    change = false;

    std::for_each(std::execution::par_unseq, counting_iterator<T>(0), counting_iterator<T>(g.size()), [&](auto u) {
      for (auto&& [v] : G[u]) {
        T comp_u = comp[u];
        T comp_v = comp[v];
        if (comp_u == comp_v) continue;
        T high_comp = comp_u > comp_v ? comp_u : comp_v;
        T low_comp  = comp_u + (comp_v - high_comp);
        if (high_comp == comp[high_comp]) {
          change          = true;
          comp[high_comp] = low_comp;
        }
      }
    });

    for (T n = 0; n < g.size(); n++) {
      while (comp[n] != comp[comp[n]]) {
        comp[n] = comp[comp[n]];
      }
    }
  }

  return comp;
}

template <typename Graph, typename T = vertex_id_t<Graph>>
auto sv_v8(Graph& g) {
  std::vector<T> comp(g.size());

  std::for_each(std::execution::par_unseq, counting_iterator<T>(0), counting_iterator<T>(g.size()), [&](auto n) { comp[n] = n; });

  bool change = true;

  auto G = g.begin();

  for (size_t num_iter = 0; num_iter < g.size(); ++num_iter) {
    if (false == change) {
      std::cout << "Shiloach-Vishkin took " << num_iter << " iterations" << std::endl;
      break;
    }
    change = false;

    std::for_each(std::execution::par_unseq, counting_iterator<T>(0), counting_iterator<T>(g.size()), [&](auto u) {
      auto Gu = G[u];
      // tbb::parallel_for(G[u], [&] (auto& Gu) {
      for (auto&& [v] : Gu) {
        T comp_u = comp[u];
        T comp_v = comp[v];
        if (comp_u == comp_v) continue;
        T high_comp = std::max(comp_u, comp_v);

        T low_comp = comp_u + (comp_v - high_comp);
        if (high_comp == comp[high_comp]) {
          change          = true;
          comp[high_comp] = low_comp;
        }
      }
      // });
    });

    for (T n = 0; n < g.size(); n++) {
      while (comp[n] != comp[comp[n]]) {
        comp[n] = comp[comp[n]];
      }
    }
  }

  return comp;
}

template <typename Graph, typename T = vertex_id_t<Graph>>
auto sv_v9(Graph& g) {
  std::vector<T> comp(g.size());

  std::for_each(std::execution::par_unseq, counting_iterator<T>(0), counting_iterator<T>(g.size()), [&](auto n) { comp[n] = n; });

  bool change = true;

  for (size_t num_iter = 0; num_iter < g.size(); ++num_iter) {
    if (false == change) {
      std::cout << "Shiloach-Vishkin took " << num_iter << " iterations" << std::endl;
      break;
    }
    change = false;

    tbb::parallel_for(edge_range(g), [&](auto&& sub) {
      for (auto&& [u, v] : sub) {
        T comp_u = comp[u];
        T comp_v = comp[v];
        if (comp_u == comp_v) continue;
        T high_comp = std::max(comp_u, comp_v);

        T low_comp = comp_u + (comp_v - high_comp);
        if (high_comp == comp[high_comp]) {
          change          = true;
          comp[high_comp] = low_comp;
        }
      }
    });

    for (T n = 0; n < g.size(); n++) {
      while (comp[n] != comp[comp[n]]) {
        comp[n] = comp[comp[n]];
      }
    }
  }

  return comp;
}

}    // namespace graph
}    // namespace nw
#endif    // CONNECTED_COMPONENT_EXPERIMENTAL_HPP