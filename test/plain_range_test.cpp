//
// This file is part of BGL17 (aka NWGraph aka GraphPack aka the Graph Standard Library)
// (c) Pacific Northwest National Laboratory 2018
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#include <iostream>
#include <queue>

#include "compressed.hpp"
#include "edge_list.hpp"

#include "io/mmio.hpp"
#include "edge_range.hpp"
#include "plain_range.hpp"

using namespace nw::graph;
using namespace nw::util;

int main(int argc, char* argv[]) {

  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " adj.mmio [ num_points ] [ seed ]" << std::endl;
    return -1;
  }

  auto                           aos_a = read_mm<directed>(argv[1]);
  compressed_sparse<0, directed> A(aos_a);

  for (auto&& [y] : plain_range(A)) {
    std::cout << y << std::endl;
  }

  for (auto&& [x, y] : edge_range(A)) {
    std::cout << x << " " << y << std::endl;
  }

  return 0;
}
