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

#ifndef NW_GRAPH_SPMATSPMAT_HPP
#define NW_GRAPH_SPMATSPMAT_HPP

#include <algorithm>
#include <map>
#include <tuple>

#include "nwgraph/adaptors/plain_range.hpp"
#include "nwgraph/edge_list.hpp"
#include "nwgraph/util/util.hpp"

namespace nw {
namespace graph {

//****************************************************************************
// A * B
//****************************************************************************

//****************************************************************************
/// @todo cannot currently pass "const &" for A or B
/// @todo Need to discuss interface options
template <typename ScalarT, typename LGraphT, typename RGraphT, typename MapOpT = std::multiplies<ScalarT>,
          typename ReduceOpT = std::plus<ScalarT>>
edge_list<directedness::directed, ScalarT> spMatspMat(LGraphT& A, RGraphT& B) {
  edge_list<directedness::directed, ScalarT> edges(0);
  edges.open_for_push_back();

  using vertex_id_type = vertex_id_t<LGraphT>;

  // compute A * B
  vertex_id_type i = 0;    // row_it - A.begin();
  for (auto row_it = A.begin(); row_it != A.end(); ++row_it, ++i) {
    // clear Row i of C
    std::map<size_t, ScalarT> Ci_tmp;

    for (auto const Ai_elt : *row_it) {
      auto k    = std::get<0>(Ai_elt);
      auto a_ik = std::get<1>(Ai_elt);

      for (auto const Bk_elt : *(B.begin() + k)) {
        auto j    = std::get<0>(Bk_elt);
        auto b_kj = std::get<1>(Bk_elt);

        // TODO: Do we really want semiring support.  If so,
        // what is best way to deal with additive identity?
        ScalarT tmp(MapOpT()(a_ik, b_kj));    // C_ij partial product

        if (Ci_tmp.find(j) != Ci_tmp.end()) {
          Ci_tmp[j] = ReduceOpT()(Ci_tmp[j], tmp);
        } else {
          Ci_tmp[j] = tmp;
        }
      }
    }
    // extract from the map and put in edge_list
    for (auto const elt : Ci_tmp) {
      edges.push_back(i, elt.first, elt.second);
    }
  }

  edges.close_for_push_back();
  sort_by<0>(edges);
  return edges;
}

//****************************************************************************
// A * B'
//****************************************************************************

//****************************************************************************
// map
template <class InputIt1, class InputIt2, class Output, class Compare, class Map>
void set_ewise_intersection(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2, Output& container, Compare comp, Map map) {
  while (first1 != last1 && first2 != last2) {
    if (comp(*first1, *first2)) {
      ++first1;
    } else {
      if (!comp(*first2, *first1)) {
        container.push_back(map(*first1, *first2));
        // std::cout << "map = " << map(*first1, *first2) << std::endl;
        ++first1;
      }
      ++first2;
    }
  }
}

//****************************************************************************
/// @todo cannot currently pass "const &" for A or BT
template <typename ScalarT, typename LGraphT, typename RGraphT, typename MapOpT = std::multiplies<ScalarT>,
          typename ReduceOpT = std::plus<ScalarT>>
edge_list<directedness::directed, ScalarT> spMatspMatT(LGraphT& A, RGraphT& BT) {
  std::vector<ScalarT> products;

  using vertex_id_type = vertex_id_t<LGraphT>;

  edge_list<directedness::directed, ScalarT> edges(0);
  edges.open_for_push_back();

  // compute A * B' with a series of sparse dot products
  vertex_id_type row_idx = 0;
  for (auto row_it = A.begin(); row_it != A.end(); ++row_it, ++row_idx) {
    vertex_id_type col_idx = 0;
    for (auto col_it = BT.begin(); col_it != BT.end(); ++col_it, ++col_idx) {
      // std::cout << "Computing " << row_idx << "," << col_idx << std::endl;
      products.clear();

      set_ewise_intersection((*row_it).begin(), (*row_it).end(), (*col_it).begin(), (*col_it).end(), products,
                             [](auto&& a, auto&& bt) -> bool { return std::get<0>(a) < std::get<0>(bt); },
                             [](auto&& a, auto&& bt) -> ScalarT { return MapOpT()(std::get<1>(a), std::get<1>(bt)); });

      if (!products.empty()) {
        ScalarT res = std::accumulate(products.begin(), products.end(), (ScalarT)0, ReduceOpT());
        edges.push_back(row_idx, col_idx, res);
        // std::cout << "Added element (" << row_idx << ","
        //          << col_idx << ") = " << res << std::endl;
        products.clear();
      }
    }
  }

  edges.close_for_push_back();
  // edges.template sort_by<0>();  // not necessary
  return edges;
}
}    // namespace graph
}    // namespace nw

#endif    // NW_GRAPH_SPMATSPMAT_HPP