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

#include "nwgraph/adaptors/bfs_range.hpp"

namespace nw {
namespace graph {

template <typename DistanceT, typename GraphT>
std::vector<DistanceT> prim(const GraphT& graph, vertex_id_type source) {}


#endif    // NW_GRAPH_PRIM_HPP
