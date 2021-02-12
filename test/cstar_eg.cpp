//
// This file is part of BGL17 (aka NWGraph aka GraphPack aka the Graph Standard Library)
// (c) Pacific Northwest National Laboratory 2018
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#include "csr.hpp"
#include "csc.hpp"
#include "coo.hpp"


using namespace nw::graph;

int main() {
  size_t N = 5;

  csc_graph A(N);
  csr_graph B(N);
  coo_graph C(N);

  csc_matrix D(N);
  csr_matrix E(N);
  coo_matrix<double> F(N);  // coo_matrix F(N) causes internal compiler error gcc 10.2.0

  csc_matrix<float> G(N);
  csr_matrix<float> H(N);
  coo_matrix<float> I(N);

  return 0;
}
