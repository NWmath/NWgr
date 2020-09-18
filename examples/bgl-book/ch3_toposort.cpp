//
// This file is part of BGL17 (aka NWGraph aka GraphPack aka the Graph Standard Library)
// (c) Pacific Northwest National Laboratory 2018
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#include "compressed.hpp"
#include "edge_list.hpp"
#include "dfs_range.hpp"
#include "mmio.hpp"

int main() {

  edge_list<directed> E = read_mm<directed>("makefile-dependencies.mmio");
  adjacency<0> A(E);

  std::vector<std::string> name;
  std::ifstream name_in("makefile-target-names.dat");
  while(!name_in.eof()) {
    std::string buffer;
    name_in >> buffer;
    name.push_back(buffer);
  } 
  
  std::vector<size_t> order;
  for (auto&& j : dfs_range(A, 0)) {
    //    std::cout << j << std::endl;
    std::cout << name[j] << std::endl;
    order.push_back(j);
  }  

  return 0;
}