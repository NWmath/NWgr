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

#include "containers/edge_list.hpp"
#include "io/mmio.hpp"

using namespace nw::graph;
using namespace nw::util;

int main(int argc, char* argv[]) {

  if (argc != 2) {
    std::cout << argv[0] << " mtx file" << std::endl;
  }

  auto aos_a = read_mm<directedness::undirected>(argv[1]);

  std::ofstream outfile("foo.bin");
  aos_a.serialize(outfile);
  outfile.close();
  write_mm("foo.mtx", aos_a, "symmetric");

  decltype(aos_a) aos_b(0);
  std::ifstream   infile("foo.bin");
  aos_b.deserialize(infile);
  infile.close();

  write_mm("bar.mtx", aos_b, "symmetric");

  aos_a.swap_to_triangular<0, predecessor>();
  aos_a.lexical_sort_by<0>();
  aos_a.uniq();

  aos_a.swap_to_triangular<0, predecessor>();
  aos_a.lexical_sort_by<0>();
  aos_a.uniq();

  compressed_sparse<0, undirected> A(aos_a);

  std::ofstream csr_outfile("foo.csr");
  A.serialize(csr_outfile);
  csr_outfile.close();

  compressed_sparse<0, undirected> B(0);
  std::ifstream                    csr_infile("foo.csr");
  B.deserialize(csr_infile);
  csr_infile.close();

  std::ofstream csr_outfile_2("bar.csr");
  B.serialize(csr_outfile_2);
  csr_outfile_2.close();

  return 0;
}
