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

#ifndef NW_GRAPH_PAGE_RANK_HPP
#define NW_GRAPH_PAGE_RANK_HPP

#include <cmath>
#include <future>
#include <iomanip>
#include <iostream>
#include <memory>
#include <tuple>
#include <vector>

#include "nwgraph/graph_concepts.hpp"
#include "nwgraph/adaptors/edge_range.hpp"
#include "nwgraph/containers/compressed.hpp"
#include "nwgraph/edge_list.hpp"
#include "nwgraph/util/parallel_for.hpp"
#include "nwgraph/adaptors/vertex_range.hpp"

namespace nw {
namespace graph {

namespace page_rank {
/**
 * @brief Helper to trace some timing.
 * 
 */
constexpr auto trace = [](auto iter, auto error, auto time, auto max) {
  auto p = std::cout.precision(10);
  auto f = std::cout.setf(std::ios::fixed, std::ios::floatfield);
  std::cout << std::setw(5) << std::left << iter;
  std::cout << std::setw(20) << std::left << error;
  std::cout << std::setw(20) << std::left << time;
  std::cout << std::setw(20) << std::left << max;
  std::cout << "\n";
  std::cout.precision(p);
  std::cout.setf(f, std::ios::floatfield);
};
/**
 * @brief Helper to time an operation.
 * 
 * @tparam Op the type of operation
 * @param op operation to time
 * @return auto a tuple of time duration and the result of the op (optional)
 */
template <class Op>
auto time_op(Op&& op) {
  if constexpr (std::is_void_v<decltype(op())>) {
    auto start = std::chrono::high_resolution_clock::now();
    op();
    std::chrono::duration<double> end = std::chrono::high_resolution_clock::now() - start;
    return std::tuple{end.count()};
  } else {
    auto                          start = std::chrono::high_resolution_clock::now();
    auto                          e     = op();
    std::chrono::duration<double> end   = std::chrono::high_resolution_clock::now() - start;
    return std::tuple{end.count(), e};
  }
}
}    // namespace page_rank

/**
 * @brief Parallel page rank.
 * 
 * @tparam Graph adjacency_list_graph graph type
 * @tparam Real page rank score type
 * @param graph input graph
 * @param degrees degree distribution of all vertices
 * @param page_rank container for page rank scores
 * @param damping_factor the probability that an imaginary surfer stops clicking
 * @param threshold error threshold to control converge rate
 * @param max_iters maximum number of iterations to converge
 * @param num_threads number of threads
 */
template <adjacency_list_graph Graph, typename Real>
[[gnu::noinline]] void page_rank_v11(const Graph& graph, const std::vector<typename Graph::vertex_id_type>& degrees,
                                     std::vector<Real>& page_rank, Real damping_factor, Real threshold, size_t max_iters, size_t num_threads) {
  std::size_t N          = graph.size();
  Real        init_score = 1.0 / N;
  Real        base_score = (1.0 - damping_factor) / N;

  {
    nw::util::life_timer _("init page rank");

    // Initialize the page rank.
    tbb::parallel_for(tbb::blocked_range(0ul, N), [&](auto&& r) {
      for (auto i = r.begin(), e = r.end(); i != e; ++i) {
        page_rank[i] = init_score;
      }
    });
  }

  std::unique_ptr<Real[]> outgoing_contrib(new Real[N]);

  page_rank::trace("iter", "error", "time", "outgoing");

  {
    nw::util::life_timer _("init contrib");

    tbb::parallel_for(tbb::blocked_range(0ul, N), [&](auto&& r) {
      for (auto i = r.begin(), e = r.end(); i != e; ++i) {
        outgoing_contrib[i] = page_rank[i] / degrees[i];
      }
    });
  }

  for (size_t iter = 0; iter < max_iters; ++iter) {

    auto&& [time, error] = page_rank::time_op([&] {
      return tbb::parallel_reduce(
          tbb::blocked_range(0ul, N), 0.0,
          [&](auto&& r, auto partial_sum) {
            for (size_t i = r.begin(), e = r.end(); i != e; ++i) {
              Real z = 0.0;
              for (auto&& j : graph[i]) {
                z += outgoing_contrib[std::get<0>(j)];
              }
              auto old_rank = page_rank[i];
              page_rank[i]  = base_score + damping_factor * z;
              partial_sum += fabs(page_rank[i] - old_rank);
              outgoing_contrib[i] = page_rank[i] / (Real)degrees[i];
            }
            return partial_sum;
          },
          std::plus{});
    });

    page_rank::trace(iter, error, time, 0);

    if (error < threshold) {
      return;
    }
  }
}

}    // namespace graph
}    // namespace nw
#endif    //  NW_GRAPH_PAGE_RANK_HPP
