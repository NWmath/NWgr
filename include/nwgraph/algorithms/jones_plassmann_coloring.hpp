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

#include "nwgraph/access.hpp"
#include "nwgraph/adaptors/edge_range.hpp"
#include "nwgraph/adaptors/plain_range.hpp"
#include "nwgraph/adaptors/dag_range.hpp"


namespace nw {
namespace graph {

template <typename Graph>
void jones_plassmann_coloring(Graph A, std::vector<vertex_id_t<Graph>>& colors) {
  using vertex_type = vertex_id_t<Graph>;
  vertex_type N = num_vertices(A);
#ifdef PRINT_DEBUG
  std::cout << "size: " << N << std::endl;
#endif

  std::vector<vertex_type> degrees(N, 0);
  // std::vector<size_t> colors(N, std::numeric_limits<std::uint32_t>::max());

  std::vector<std::vector<vertex_type>> predecessor_list(N);
  std::vector<std::vector<vertex_type>> successor_list(N);
  std::vector<vertex_type>              degree_list(N, 0);
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

  std::vector<std::vector<vertex_type>> pred_colors(N);

  /*TODO: we probably need a vertex range. */
  for (vertex_type vtx = 0; vtx < N; vtx++) {
    pred_colors[vtx].resize(predecessor_list[vtx].size(), 0);
  }

  for (vertex_type vtx = 0; vtx < N; vtx++) {
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
      vertex_type pred_count = predecessor_list[u].size();

      /*Find the minimum available color*/
      vertex_type min_color = pred_count;
      for (auto it = pred_colors[u].begin(); it < pred_colors[u].end(); it++) {
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
        vertex_type preds = predecessor_list[succ].size();
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
