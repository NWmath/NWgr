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

#ifndef NW_GRAPH_CSR_HPP
#define NW_GRAPH_CSR_HPP

#include "containers/adjacency.hpp"

namespace nw {
  namespace graph {

using csr_graph = adjacency<0>;

template <typename T = double>
using csr_matrix = adjacency<0, T>;

  }
}

#endif // NW_GRAPH_CSR_HPP
