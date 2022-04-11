// This material was prepared as an account of work sponsored by an agency of the 
// United States Government.  Neither the United States Government nor the United States 
// Department of Energy, nor Battelle, nor any of their employees, nor any jurisdiction or 
// organization that has cooperated in the development of these materials, makes any 
// warranty, express or implied, or assumes any legal liability or responsibility for the 
// accuracy, completeness, or usefulness or any information, apparatus, product, software, 
// or process disclosed, or represents that its use would not infringe privately owned rights.
//
// Reference herein to any specific commercial product, process, or service by trade name, 
// trademark, manufacturer, or otherwise does not necessarily constitute or imply its 
// endorsement, recommendation, or favoring by the United States Government or any 
// agency thereof, or Battelle Memorial Institute. The views and opinions of authors 
// expressed herein do not necessarily state or reflect those of the United States Government 
// or any agency thereof.
//                        PACIFIC NORTHWEST NATIONAL LABORATORY
//                                     operated by
//                                     BATTELLE
//                                     for the
//                          UNITED STATES DEPARTMENT OF ENERGY
//                          under Contract DE-AC05-76RL01830
//
// Authors:
//     Andrew Lumsdaine
//     Kevin Deweese
//

#ifndef NW_GRAPH_TRIANGLE_COUNT_HPP
#define NW_GRAPH_TRIANGLE_COUNT_HPP

#include "nwgraph/graph_concepts.hpp"
#include "nwgraph/adaptors/cyclic_range_adaptor.hpp"
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

/**
 * @brief Sequential 2D triangle counting algorithm set intersection the neighbor lists of each pair of vertices.
 * 
 * @tparam GraphT adjacency_list_graph
 * @param A graph
 * @return size_t the number of triangles
 */
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

/// Parallel triangle counting using `std::async`.
///
/// This version of triangle counting uses `threads` `std::async` launches to
/// evaluate the passed `op` in parallel. The `op` will be provided the thread
/// id, but should capture any other information required to perform the
/// decomposed work.
///
/// @tparam          Op The type of the decomposed work.
///
/// @param           op The decomposed work for each `std::async`.
///
/// @return             The += reduced total of counted triangles.
template <class Op>
std::size_t triangle_count_async(std::size_t threads, Op&& op) {
  // Launch the workers.
  std::vector<std::future<size_t>> futures(threads);
  for (std::size_t tid = 0; tid < threads; ++tid) {
    futures[tid] = std::async(std::launch::async, op, tid);
  }

  // Reduce the outcome.
  int         i         = 0;
  std::size_t triangles = 0;
  for (auto&& f : futures) {
    triangles += f.get();
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

/**
 * @brief Two-dimensional triangle counting.
 * 
 * @tparam Graph adjacency_list_graph
 * @param G graph
 * @param threads number of threads
 * @return std::size_t number of triangles
 */
template <adjacency_list_graph Graph>
[[gnu::noinline]] std::size_t triangle_count_v4a(const Graph& G, std::size_t threads = 1) {
  auto first = G.begin();
  auto last = G.end();
  return triangle_count_async(threads, [&](std::size_t tid) {
    std::size_t triangles = 0;
    for (auto i = first + tid; i < last; i += threads) {
      for (auto j = (*i).begin(), end = (*i).end(); j != end; ++j) {
        // assert(j < end);
        triangles += nw::graph::intersection_size(j, end, first[target(G, *j)]);
      }
    }
    return triangles;
  });
}

}    // namespace graph
}    // namespace nw

#endif    //  NW_GRAPH_TRIANGLE_COUNT_HPP
