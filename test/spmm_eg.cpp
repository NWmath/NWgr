//
// This file is part of Standard Graph Library (SGL)
// (c) Pacific Northwest National Laboratory 2018
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Scott McMillan
//

//#pragma once

#include <algorithm>
#include <tuple>

#include "algorithms/spMatspMat.hpp"
#include "containers/aos.hpp"
#include "containers/compressed.hpp"
#include "io/mmio.hpp"
#include "util/util.hpp"

using namespace nw::graph;
using namespace nw::util;

//****************************************************************************
int main(int argc, char* argv[]) {
  // call with data/spmatA.mmio and data/spmatB.mmio
  if (argc < 3) {
    std::cerr << "Usage: " << argv[0] << " <matA.mmio> <matB.mmio>" << std::endl;
    return -1;
  }

  auto aos_A = read_mm<directedness::directed, float>(argv[1]);
  auto aos_B = read_mm<directedness::directed, float>(argv[2]);
  std::cout << "A = " << std::endl;
  aos_A.stream(std::cout);
  std::cout << "B = " << std::endl;
  aos_B.stream(std::cout);

  adjacency<0, float> A(aos_A);
  adjacency<0, float> B(aos_B);

  std::cout << "C = A * B = " << std::endl;
  auto edges(spMatspMat<float>(A, B));
  edges.stream(std::cout);

  std::cout << "C = A * B' = " << std::endl;
  auto edgesABT(spMatspMatT<float>(A, B));
  edgesABT.stream(std::cout);
}
