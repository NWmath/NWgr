
//
// This file is part of Standard Graph Library (SGL)
// (c) Pacific Northwest National Laboratory 2018
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#include <iostream>
#include <vector>

#include "nwgraph/util/proxysort.hpp"

namespace nw::graph {};

using namespace nw::graph;
using namespace nw::util;

template <typename Vector>
void print_n_ranks(const Vector& page_rank, size_t n) {
  auto perm = proxysort<size_t>(page_rank, std::greater<float>());
  for (size_t i = 0; i < n; ++i) {
    std::cout << std::to_string(perm[i]) + ": " << std::to_string(page_rank[perm[i]]) << std::endl;
  }
}

int main() {

  std::vector<float> x{3., 1., .14, .15, 9};

  print_n_ranks(x, x.size());

  return 0;
}
