//
// This file is part of BGL17 (aka NWGraph aka GraphPack aka the Graph Standard Library)
// (c) Pacific Northwest National Laboratory 2018
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#include "binio.hpp"
#include "edge_list.hpp"
#include "io/mmio.hpp"
#include <iostream>

using namespace nw::graph;
using namespace nw::util;

void usage(const std::string& msg) { std::cout << msg << std::endl; }

int main(int argc, char* argv[]) {

  bool symmetric         = false;

  std::string format   = "edgelist";
  std::string mtx_file = "";
  std::string out_file = "";

  for (int argIndex = 1; argIndex < argc; ++argIndex) {
    std::string arg(argv[argIndex]);

    if (arg == "-i") {
      if (++argIndex == argc) {
        usage(argv[0]);
      }
      mtx_file = std::string(argv[argIndex]);
    } else if (arg == "-o") {
      if (++argIndex == argc) {
        usage(argv[0]);
      }
      out_file = std::string(argv[argIndex]);
    } else if (arg == "-f") {
      if (++argIndex == argc) {
        usage(argv[0]);
      }
      format = std::string(argv[argIndex]);
    } else if (arg == "-s") {
      symmetric = true;
    }
  }

  if (mtx_file == "" || out_file == "") {
    usage(argv[0]);
    return -1;
  }

  directedness       input_sym  = is_mm(mtx_file) ? get_mm_symmetry(mtx_file) : directed;
  const directedness output_sym = symmetric ? undirected : directed;

  if (input_sym == output_sym) {
    if (input_sym == directed) {
      auto aos_a = is_mm(mtx_file) ? read_mm<directed>(mtx_file) : read_bin<directed>(mtx_file);
      write_bin(out_file, aos_a);
    } else {
      auto aos_a = is_mm(mtx_file) ? read_mm<undirected>(mtx_file) :read_bin<undirected>(mtx_file);
      write_bin(out_file, aos_a);
    }
  } else {
    if (input_sym == directed) {
      auto aos_a = is_mm(mtx_file) ? read_mm<directed>(mtx_file) : read_bin<directed>(mtx_file);
      auto aos_b = aos_a.convert_directedness<undirected>();
      write_bin(out_file, aos_b);
    } else {
      auto aos_a = is_mm(mtx_file) ? read_mm<undirected>(mtx_file) : read_bin<undirected>(mtx_file);
      auto aos_b = aos_a.convert_directedness<directed>();
      write_bin(out_file, aos_b);
    }
  }

#if 0  
  auto aos_a = read_mm<directed>(mtx_file);
  auto x     = aos_a.convert_directedness<directed>();
  auto y     = aos_a.convert_directedness<undirected>();

  auto aos_b = read_mm<undirected>(mtx_file);
  auto x     = aos_a.convert_directedness<directed>();
  auto y     = aos_a.convert_directedness<undirected>();
  if (remove_self_loops) {
    aos_a.remove_self_loops();
  }
  if (format == "edgelist" || format == "edge_list") {
    write_bin(out_file, aos_a);
  }
        
  if (format == "csr") {
    aos_a.triangularize<predecessor>();
    aos_a.lexical_sort_by<0>();
    aos_a.uniq();
    compressed_sparse<0, undirected> A(aos_a);

    std::ofstream outfile(out_file);
    A.serialize(outfile);
    outfile.close();
  } else if (format == "edgelist" || format == "edge_list") {
    write_bin(out_file, aos_a).
  } else {
    usage(argv[1]);
    return -1;
  }
#endif

  return 0;
}
