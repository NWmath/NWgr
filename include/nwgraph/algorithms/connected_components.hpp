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

#ifndef CONNECTED_COMPONENT_HPP
#define CONNECTED_COMPONENT_HPP

#include "nwgraph/adaptors/bfs_edge_range.hpp"
#include "nwgraph/adaptors/edge_range.hpp"
#include "nwgraph/adaptors/vertex_range.hpp"
#include "nwgraph/util/atomic.hpp"
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

template <typename Vector, typename T>
static void link(T u, T v, Vector& comp) {
  T p1 = nw::graph::acquire(comp[u]);
  T p2 = comp[v];
  while (p1 != p2) {
    T high   = std::max(p1, p2);
    T low    = p1 + (p2 - high);
    T p_high = comp[high];

    if ((p_high == low) || (p_high == high && comp[high].compare_exchange_strong(high, low))) break;
    p1 = comp[p_high];
    p2 = comp[low];
  }
}

template <typename Execution, typename Graph, typename Vector>
static void compress(Execution exec, const Graph& g, Vector& comp) {
  std::for_each(exec, counting_iterator(0ul), counting_iterator(comp.size()), [&](auto n) {
    while (comp[n] != comp[comp[n]]) {
      auto foo = nw::graph::acquire(comp[n]);
      auto bar = nw::graph::acquire(comp[foo]);
      nw::graph::release(comp[n], bar);
    }
  });
}

template <typename Vector, typename T>
static T sample_frequent_element(const Vector& comp, size_t num_samples = 1024) {
  std::unordered_map<T, int>       counts(32);
  std::mt19937                                  gen;
  std::uniform_int_distribution<T> distribution(0, comp.size() - 1);

  for (size_t i = 0; i < num_samples; ++i) {
    T n = distribution(gen);
    counts[comp[n]]++;
  }

  auto&& [num, count] = *std::max_element(counts.begin(), counts.end(), [](auto&& a, auto&& b) { return std::get<1>(a) < std::get<1>(b); });
  float frac_of_graph = static_cast<float>(count) / num_samples;
  std::cout << "Skipping largest intermediate component (ID: " << num << ", approx. " << int(frac_of_graph * 100) << "% of the graph)\n";
  return num;
}

template <typename Execution, typename Graph1, typename Graph2>
static auto afforest(Execution&& exec, Graph1&& graph, Graph2&& t_graph, size_t neighbor_rounds = 2) {
  using vertex_id_type = vertex_id_t<Graph1>;
  std::vector<std::atomic<vertex_id_type>> comp(graph.size() + 1);
  std::for_each(exec, counting_iterator(0ul), counting_iterator(comp.size()), [&](vertex_id_type n) { comp[n] = n; });
  auto g = graph.begin();
  for (size_t r = 0; r < neighbor_rounds; ++r) {
    std::for_each(exec, counting_iterator(0ul), counting_iterator(comp.size()), [&](vertex_id_type u) {
      if (r < (g[u]).size()) {
        link(u, std::get<0>(g[u].begin()[r]), comp);
      }
    });
    compress(exec, graph, comp);
  }

  vertex_id_type c = sample_frequent_element<std::vector<std::atomic<vertex_id_type>>, vertex_id_type>(comp);

  std::for_each(exec, counting_iterator(0ul), counting_iterator(comp.size()), [&](vertex_id_type u) {
    if (comp[u] == c) return;

    if (neighbor_rounds < g[u].size()) {
      for (auto v = g[u].begin() + neighbor_rounds; v != g[u].end(); ++v) {
        link(u, std::get<0>(*v), comp);
      }
    }

    if (t_graph.size() != 0) {
      for (auto&& [v] : (t_graph.begin())[u]) {
        link(u, v, comp);
      }
    }
  });

  compress(exec, g, comp);

  return comp;
}

}    // namespace graph
}    // namespace nw
#endif    // CONNECTED_COMPONENT_HPP
