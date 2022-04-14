/**
 * @file jaccard_eg.cpp
 *
 * @copyright SPDX-FileCopyrightText: 2022 Batelle Memorial Institute
 * @copyright SPDX-FileCopyrightText: 2022 University of Washington
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * @authors
 *   Tony Liu
 *
 */

//
// This file is part of BGL17 (aka NWGraph aka GraphPack aka the Graph Standard Library)
// (c) Pacific Northwest National Laboratory 2018
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Xu Tony Liu
//


#include <iostream>
#include <queue>

#include "nwgraph/build.hpp"
#include "nwgraph/algorithms/jaccard.hpp"
#include "nwgraph/containers/compressed.hpp"
#include "nwgraph/edge_list.hpp"
#include "nwgraph/io/mmio.hpp"

#include <type_traits>

using namespace nw::graph;
using namespace nw::util;


//****************************************************************************
int main(int argc, char* argv[]) {

  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " adj.mmio " << std::endl;
    return -1;
  }

  edge_list<directedness::directed, size_t> aos_a = read_mm<directedness::directed, size_t>(argv[1]);
  adjacency<0, size_t> A(aos_a.num_vertices()[0]);
  push_back_fill(aos_a, A);  // from build.hpp

  auto Weight = [](auto& e) -> auto& {
    return std::get<1>(e);
  };
  size_t js_score = jaccard_similarity_v0<>(A, Weight);

  return 0;
}
