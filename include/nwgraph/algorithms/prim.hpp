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
//

#ifndef NW_GRAPH_PRIM_HPP
#define NW_GRAPH_PRIM_HPP

#include "nwgraph/graph_concepts.hpp"
#include "nwgraph/graph_traits.hpp"
#include "nwgraph/edge_list.hpp"

#include <vector>

namespace nw {
namespace graph {

/**
 * @brief Prim's minimum spanning tree algorithm
 * A greedy algorithm for a weighted undirected graph
 * @tparam Graph Graph type.  Must meet requirements of adjacency_list_graph.
 * @tparam Distance Type of edge weight.
 * @tparam Weight A weight function for a given edge, returns a Distance.
 * @param graph Input graph.
 * @param source Starting vertex.
 * @return std::vector<vertex_id_t<Graph>>, the predecessor list (the MST).
 */
template <adjacency_list_graph Graph, class Distance, class Weight>
std::vector<vertex_id_t<Graph>> prim(const Graph& graph, vertex_id_t<Graph> source, Weight&& weight) {

  using vertex_id_type = vertex_id_t<Graph>;

  size_t N { num_vertices(graph) };
  assert(source < N);

  std::vector<Distance> distance(N, std::numeric_limits<Distance>::max());
  std::vector<Distance> predecessor(N, std::numeric_limits<Distance>::max());
  std::vector<uint8_t> finished(N, false);
  distance[source] = 0;

  using weight_t        = Distance;
  using weighted_vertex = std::tuple<vertex_id_type, weight_t>;

  std::priority_queue<weighted_vertex, std::vector<weighted_vertex>, std::greater<weighted_vertex>> Q;

  while (!Q.empty()) {

    auto u = std::get<0>(Q.top());
    Q.pop();

    if (finished[u]) {
      continue;
    }

    std::for_each(g[u].begin(), g[u].end(), [&](auto&& e) {
      auto v = target(graph, e);
      auto w = weight(e);

      if (!finished[v] && distance[v] > w) {
	distance[v] = w;
	Q.push({ v, distance[v] });
	predecessor[v] = u;
      }
    });
  }

  return predecessor;
}

}    // namespace graph
}    // namespace nw
#endif    // NW_GRAPH_PRIM_HPP
