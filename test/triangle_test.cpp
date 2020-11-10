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

#include <algorithms/triangle_count.hpp>
#include <compressed.hpp>
#include <edge_list.hpp>
#include <io/mmio.hpp>

#include "common/test_header.hpp"

using namespace nw::graph;
using namespace nw::util;


typedef compressed_sparse<0, undirected> csr_graph;

//****************************************************************************
TEST_CASE("triangle counting", "[tc]") {

  auto aos_a = read_mm<undirected>(DATA_DIR "karate.mtx");
  aos_a.swap_to_triangular<0, successor>();
  aos_a.sort_by<1>();
  aos_a.stable_sort_by<0>();

  csr_graph A(aos_a);

  SECTioN("v0") {
    size_t triangles = triangle_count_v0(A);
    std::cout << triangles << " triangles (v0)\n";
    REQUIRE(triangles == 45);
  }

  SECTioN("v1") {
    size_t triangles = triangle_count_v1(A);
    std::cout << triangles << " triangles (v1)\n";
    REQUIRE(triangles == 45);
  }

  SECTioN("v2") {
    size_t triangles = triangle_count_v2(A);
    std::cout << triangles << " triangles (v2)\n";
    REQUIRE(triangles == 45);
  }

  SECTioN("v3") {
    size_t triangles = triangle_count_v3(A);
    std::cout << triangles << " triangles (v3)\n";
    REQUIRE(triangles == 45);
  }

  SECTioN("v4") {
    size_t triangles = triangle_count_v4(A.begin(), A.end());
    std::cout << triangles << " triangles (v4)\n";
    REQUIRE(triangles == 45);
  }

  SECTioN("v5") {
    size_t triangles = triangle_count_v5(A.begin(), A.end());
    std::cout << triangles << " triangles (v5)\n";
    REQUIRE(triangles == 45);
  }

  SECTioN("v6") {
    size_t triangles = triangle_count_v6(A.begin(), A.end());
    std::cout << triangles << " triangles (v6)\n";
    REQUIRE(triangles == 45);
  }

  SECTioN("v7") {
    size_t triangles = triangle_count_v7(A);
    std::cout << triangles << " triangles (v7)\n";
    REQUIRE(triangles == 45);
  }

  SECTioN("v10") {
    size_t triangles = triangle_count_v10(A);
    std::cout << triangles << " triangles (v10)\n";
    REQUIRE(triangles == 45);
  }

  SECTioN("v12") {
    size_t triangles = triangle_count_v12(A, 1);
    std::cout << triangles << " triangles (v12)\n";
    REQUIRE(triangles == 45);
  }

  SECTioN("v13") {
    size_t triangles = triangle_count_v13(A, 1);
    std::cout << triangles << " triangles (v13)\n";
    REQUIRE(triangles == 45);
  }

  SECTioN("v14") {
    size_t triangles = triangle_count_v14(A);
    std::cout << triangles << " triangles (v14)\n";
    REQUIRE(triangles == 45);
  }

}
