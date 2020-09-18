//
// This file is part of BGL17 (aka NWGraph aka GraphPack aka the Graph Standard Library)
// (c) Pacific Northwest National Laboratory 2018
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Kevin Deweese
//

#include <algorithm>
#include <compressed.hpp>
#include <aolos.hpp>
#include <edge_list.hpp>
#include <mmio.hpp>
#include <vector>

#include "common/test_header.hpp"

using namespace nw::graph;

TEST_CASE("Size Test", "[size test]") {
    size_t N = 5;
    edge_list<undirected> E_list(N);
    E_list.push_back(0, 1);
    E_list.push_back(1, 2);
    E_list.push_back(2, 3);
    
    adjacency<0> A(E_list);
    REQUIRE(A.size() == 4);

    adj_list<> A_list(E_list);
    REQUIRE(A_list.size() == 4);

    edge_list<directed> E_list2(N);
    E_list2.push_back(0, 1);
    E_list2.push_back(1, 2);
    E_list2.push_back(2, 3);

    adjacency<0> A2(E_list2);
    REQUIRE(A2.size() == 4);

    adj_list<> A_list2(E_list2);
    REQUIRE(A_list2.size() == 4);
}