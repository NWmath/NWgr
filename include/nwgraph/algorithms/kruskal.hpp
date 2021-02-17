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

#ifndef NW_GRAPH_KRUSKAL_HPP
#define NW_GRAPH_KRUSKAL_HPP

#include <algorithm>
#include <tuple>

#include "nwgraph/util/disjoint_set.hpp"
#include "nwgraph/util/util.hpp"

namespace nw {
namespace graph {

template <typename EdgeListT>
EdgeListT kruskal(EdgeListT& E) {
  return kruskal(E, [](auto t1, auto t2) { return std::get<2>(t1) < std::get<2>(t2); });
}
//****************************************************************************
template <typename EdgeListT, typename Compare>
EdgeListT kruskal(EdgeListT& E, Compare comp) {
  size_t    n_vtx = E.size();
  EdgeListT T(n_vtx);
  std::sort(E.begin(), E.end(), comp);

  std::vector<std::pair<vertex_id_type, size_t>> subsets(n_vtx);
  for (size_t i = 0; i < n_vtx; ++i) {
    subsets[i].first  = i;
    subsets[i].second = 0;
  }

  for (auto y : E) {
    auto u = std::get<0>(y);
    auto v = std::get<1>(y);
    if (disjoint_union_find(subsets, u, v)) T.push_back(y);
  }

  return T;
}

}    // namespace graph
}    // namespace nw

#endif    //  NW_GRAPH_KRUSKAL_HPP
