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

#ifndef NW_GRAPH_TRIANGLE_COUNT_HPP
#define NW_GRAPH_TRIANGLE_COUNT_HPP

#include "nwgraph/graph_concepts.hpp"
#include "nwgraph/adaptors/cyclic_range_adapter.hpp"
#include "nwgraph/adaptors/neighbor_range.hpp"
#include "nwgraph/adaptors/edge_range.hpp"
#include "nwgraph/util/intersection_size.hpp"
#include "nwgraph/util/parallel_for.hpp"
#include "nwgraph/util/timer.hpp"
#include "nwgraph/util/util.hpp"

#include <atomic>
#include <future>
#include <thread>
#include <tuple>
#include <vector>

namespace nw {
namespace graph {

template <adjacency_list_graph GraphT>
size_t triangle_count_v0(const GraphT& A) {
  size_t triangles = 0;
  auto   first     = A.begin();
  auto   last      = A.end();
  for (auto u_neighors = first; u_neighors != last; ++u_neighors) {
    for (auto elt = (*u_neighors).begin(); elt != (*u_neighors).end(); ++elt) {
      auto v = target(A, *elt);
      triangles += nw::graph::intersection_size(*u_neighors, A[v]);
    }
  }
  return triangles;
}


/// Two-dimensional triangle counting.
///
/// This version of triangle counting is explicitly two-dimensional and is
/// optimized (and only correct for) an upper-triangular graph. It uses explicit
/// async threads to perform the parallelization.
///
/// @tparam RandomAccessIterator A random access BGL17 iterator type.
///
/// @param        first The beginning of the outer range.
/// @param         last The end of the outer range.
/// @param      threads The number of threads to use in the parallelization.
///
/// @returns            The number of triangles in the graph.
template <typename RandomAccessIterator>
[[gnu::noinline]] std::size_t triangle_count_v4(RandomAccessIterator first, RandomAccessIterator last, std::size_t threads = 1) {
  return triangle_count_async(threads, [&](std::size_t tid) {
    std::size_t triangles = 0;
    for (auto i = first + tid; i < last; i += threads) {
      for (auto j = (*i).begin(), end = (*i).end(); j != end; ++j) {
        // assert(j < end);
        triangles += nw::graph::intersection_size(j, end, first[std::get<0>(*j)]);
      }
    }
    return triangles;
  });
}

}    // namespace graph
}    // namespace nw

#endif    //  NW_GRAPH_TRIANGLE_COUNT_HPP
