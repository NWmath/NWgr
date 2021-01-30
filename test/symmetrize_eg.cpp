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
#include <string>

#include "containers/edge_list.hpp"
#include "io/mmio.hpp"

using namespace nw::graph;
using namespace nw::util;

void usage(const std::string& msg) { std::cout << msg << std::endl; }

int main(int argc, char* argv[]) {

  std::string el_file  = "";
  std::string out_file = "";

  for (int argIndex = 1; argIndex < argc; ++argIndex) {
    std::string arg(argv[argIndex]);

    if (arg == "-i") {
      if (++argIndex == argc) {
        usage(argv[0]);
      }
      el_file = std::string(argv[argIndex]);
    } else if (arg == "-o") {
      if (++argIndex == argc) {
        usage(argv[0]);
      }
      out_file = std::string(argv[argIndex]);
    }
  }

  edge_list<> el(0);
  el.deserialize(el_file);

  el.swap_to_triangular<0, successor>();
  el.lexical_sort_by<0>();
  el.uniq();
  el.serialize(out_file);

  return 0;
}
