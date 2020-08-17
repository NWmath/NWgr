//
// This file is part of the Graph Standard Library (aka BGL17 aka NWGraph)
// (c) Pacific Northwest National Laboratory
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#include <iostream>
#include <queue>
#include <algorithms/dijkstra.hpp>
#include <compressed.hpp>
#include <edge_list.hpp>
#include <mmio.hpp>

//****************************************************************************
int main(int argc, char* argv[]) {

  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " adj.mmio " << std::endl;
    return -1;
  }

  edge_list<directed, size_t>            aos_a = read_mm<directed, size_t>(argv[1]);
  compressed_sparse<0, directed, size_t> A(aos_a);

  //  auto distance = dijkstra<size_t>(A, 0);

  //  for (auto d : distance)
  //    std::cout << d << std::endl;

  auto distance_v0 = dijkstra_v0<size_t>(A, 0);

  for (auto d : distance_v0)
    std::cout << d << std::endl;

  return 0;
}
