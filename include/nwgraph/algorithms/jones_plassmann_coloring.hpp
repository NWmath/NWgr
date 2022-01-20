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
//

/*Implements Jones-Plassmann algorithm for coloring */
#ifndef JONES_PLASSMANN_COLORING_HPP
#define JONES_PLASSMANN_COLORING_HPP

#include "nwgraph/adaptors/dag_range.hpp"
#include "nwgraph/adaptors/edge_range.hpp"
#include "nwgraph/adaptors/plain_range.hpp"

#include "nwgraph/algorithms/mis.hpp"

namespace nw {
namespace graph {

/* Graph greedy coloring using maximal independent set. */
template <typename Graph>
void jones_plassmann_coloring(Graph A, std::vector<size_t>& colors) {
  size_t N = A.size();
  //init every nodes' color to 0
  std::fill(colors.begin(), colors.end(), 0);

  //find the maximal independent set
  std::vector<size_t> independentSet;
  mis_algorithm(A, independentSet);

  // Function for finding the first
  // missing positive number in an (unsorted) array
  auto firstMissingPositive = []<typename T>(std::vector<T> arr) -> T {
    size_t n = arr.size();
    // Loop to traverse the whole array
    for (size_t i = 0; i < n; ++i) {
      // Loop to check boundary
      // condition and for swapping
      while (1 <= arr[i] && arr[i] <= n && arr[i] != arr[arr[i] - 1]) {
        std::swap(arr[i], arr[arr[i] - 1]);
      }
    }

    // Checking any element which
    // is not equal to i+1
    for (size_t i = 0; i < n; ++i) {
      if (arr[i] != i + 1) {
        return i + 1;
      }
    }

    // Nothing is present return last index
    return n + 1;
  };

  //calculate colors based on the maximal independent set
  std::vector<std::vector<size_t>> neighbor_colors(N);
  for (auto& u : independentSet) {
    //memorize colors of every independent vertex's neighbors
    for (auto it = A[u].begin(); it != A[u].end(); ++it) {
      auto v = std::get<0>(*it);
      neighbor_colors[u].push_back(colors[v]);
    }
    //find the first unused color within every independent vertex's neighbors
    colors[u] = firstMissingPositive(neighbor_colors[u]);
  }
}

/* TODO This implemenation is buggy. Need to fix. */
template <typename Graph>
void jones_plassmann_coloring2(Graph A, std::vector<size_t>& colors) {
  size_t N = A.size();
#ifdef PRINT_DEBUG
  std::cout << "size: " << N << std::endl;
#endif

  std::vector<size_t> degrees(N, 0);
  // std::vector<size_t> colors(N, std::numeric_limits<std::uint32_t>::max());

  std::vector<std::vector<size_t>> predecessor_list(N);
  std::vector<std::vector<size_t>> successor_list(N);
  std::vector<size_t>              degree_list(N, 0);
  /*Need a "Degree range"*/
  for (auto&& [v, deg] : plain_degree_range(A)) {
#ifdef PRINT_DEBUG
    std::cout << "v: " << v + 1 << " deg: " << deg << std::endl;
#endif
    degrees[v] = deg;
  }

  /*plain edge range will be used to calculate predecessor/successors*/
  /*edge_range returns a tuple every time iterated through*/

  for (auto&& [v, u] : edge_range(A)) {
    if (degrees[v] > degrees[u]) {
      successor_list[v].push_back(u);
      predecessor_list[u].push_back(v);
    } else if (degrees[v] < degrees[u]) {
      predecessor_list[v].push_back(u);
      successor_list[u].push_back(v);
    } else {
      if (v > u) {
        predecessor_list[v].push_back(u);
        successor_list[u].push_back(v);
      } else {
        successor_list[v].push_back(u);
        predecessor_list[u].push_back(v);
      }
    }
  }

  std::vector<std::vector<uint64_t>> pred_colors(N);

  /*TODO: we probably need a vertex range. */
  for (uint64_t vtx = 0; vtx < N; vtx++) {
    pred_colors[vtx].resize(predecessor_list[vtx].size(), 0);
  }

  for (uint64_t vtx = 0; vtx < N; vtx++) {
    /*Set the color of the roots to be zero.*/
    if (predecessor_list[vtx].size() == 0) {
      colors[vtx] = 0;
#ifdef PRINT_DEBUG
      std::cout << "Setting color of " << vtx + 1 << " To " << colors[vtx] << std::endl;
#endif
      /*Update all the sucsessors*/
      for (const auto& u : successor_list[vtx]) {
        pred_colors[u][0]++;
      }
    }
  }

  for (auto&& [v, u, ready_to_process] : dag_range(A, predecessor_list, successor_list)) {
#ifdef PRINT_DEBUG
    std::cout << "V: " << v + 1 << " U:" << u + 1 << std::endl;
#endif
    if (ready_to_process == yes) {
      std::sort(pred_colors[u].begin(), pred_colors[u].end());
      uint64_t pred_count = predecessor_list[u].size();

      /*Find the minimum available color*/
      uint64_t min_color = pred_count;
      for (std::vector<uint64_t>::iterator it = pred_colors[u].begin(); it < pred_colors[u].end(); it++) {
        if (*it == 0) {
          min_color = *it;
          break;
        }
      }

      /*Set the new color to the vertex*/
      colors[u] = min_color;
#ifdef PRINT_DEBUG
      std::cout << "--->Color of " << u + 1 << " " << colors[u] << std::endl;
#endif
      /*Update all the sucsessors*/
      for (const auto& succ : successor_list[u]) {
        /*Check whether the new color by the predecessor is
      less than of its total pred count*/
        uint64_t preds = predecessor_list[succ].size();
#ifdef PRINT_DEBUG
        std::cout << "succ: " << succ << " predlist size: " << preds << std::endl;
#endif
        if (min_color < preds) {
          pred_colors[succ][min_color]++;
        }
      }
    }
  }

#ifdef PRINT_DEBUG
  for (const auto& color : colors) {
    std::cout << color << std::endl;
  }
#endif

  return;
}

}    // namespace graph
}    // namespace nw
#endif    // JONES_PLASSMANN_COLORING_HPP
