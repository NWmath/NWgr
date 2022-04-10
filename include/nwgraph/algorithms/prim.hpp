//
// This file is part of NW Graph (aka GraphPack)
// (c) Pacific Northwest National Laboratory 2018-2021
// (c) University of Washington 2018-2021
//
// Licensed under terms of include LICENSE file
//
// Authors:
//     Andrew Lumsdaine
//

#ifndef NW_GRAPH_PRIM_HPP
#define NW_GRAPH_PRIM_HPP

#include "nwgraph/graph_concepts.hpp"
#include "nwgraph/graph_traits.hpp"

namespace nw {
namespace graph {

/** 
 * Prim's minimum spanning tree algorithm
 * 
 * @tparam DistanceT
 * @tparam GraphT
 */
template <class DistanceT, class GraphT>
std::vector<DistanceT> prim(const GraphT& graph, vertex_id_t<GraphT> source) {
  std::vector<DistanceT> ret;
  return ret;
}

}    // namespace graph
}    // namespace nw
#endif    // NW_GRAPH_PRIM_HPP
