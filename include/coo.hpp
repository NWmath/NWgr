//
// This file is part of NWGraph aka GraphPack aka the Graph Standard Library
// (c) Pacific Northwest National Laboratory 2018-2021
// (c) University of Washington 2018-2021
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#ifndef NW_GRAPH_COO_HPP
#define NW_GRAPH_COO_HPP

#include "containers/edge_list.hpp"

namespace nw {
  namespace graph {

using coo_graph = edge_list<directedness::directed>;

template <typename T = double>
using coo_matrix = edge_list<directedness::directed, T>;

  }
}

#endif // NW_GRAPH_COO_HPP
