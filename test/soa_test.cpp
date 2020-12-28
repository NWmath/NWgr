//
// This file is part of BGL17 (aka NWGraph aka GraphPack aka the Graph Standard Library)
// (c) Pacific Northwest National Laboratory 2018
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#include <iterator>

#include "containers/soa.hpp"

#include "common/test_header.hpp"

using namespace nw::graph;
using namespace nw::util;

template<size_t pos, typename Iterator>
void test_assignment(Iterator iter, size_t index = 0) {
  typename std::iterator_traits<Iterator>::reference tuple  = index ? *iter : iter[index];
  std::get<pos>(tuple)                                      = 0;
  typename std::iterator_traits<Iterator>::reference tuple1 = index ? *iter : iter[index];
  REQUIRE(std::get<pos>(tuple1) == 0);
  std::get<pos>(tuple1)                                     = 1;
  typename std::iterator_traits<Iterator>::reference tuple2 = index ? *iter : iter[index];
  REQUIRE(std::get<pos>(tuple2) == 1);
}


TEST_CASE("struct of arrays", "[soa]") {
  SECTION("push back") {
    struct_of_arrays<size_t>                         E;
    struct_of_arrays<size_t, size_t>                 F;
    struct_of_arrays<size_t, size_t, double>         G;
    struct_of_arrays<double, size_t, size_t, double> H;

    E.push_back(8675309);
    F.push_back(867, 5309);
    G.push_back(867, 5309, 3.14159);
    H.push_back(95141.3, 867, 5309, 3.14159);
    H.push_back(195141.3, 1867, 15309, 13.14159);
    H.push_back(295141.3, 2867, 25309, 23.14159);
    H.push_back(395141.3, 3867, 35309, 33.14159);

    auto e = E.begin();
    REQUIRE(std::get<0>(*e) == 8675309);
    auto f = F.begin();
    REQUIRE(std::get<0>(*f) == 867);
    REQUIRE(std::get<1>(*f) == 5309);
    auto h = H.begin();
    REQUIRE(std::get<0>(*h) == 95141.3);
    REQUIRE(std::get<1>(*h) == 867);
    REQUIRE(std::get<2>(*h) == 5309);
    REQUIRE(std::get<3>(*h) == 3.14159);
    REQUIRE(std::get<0>(h[3]) == 395141.3);
    REQUIRE(std::get<1>(h[3]) == 3867);
    REQUIRE(std::get<2>(h[3]) == 35309);
    REQUIRE(std::get<3>(h[3]) == 33.14159);
  }

  SECTION("initializer list") {
    struct_of_arrays<size_t>                         E  ({ 1, 2, 3 });

    struct_of_arrays<size_t>                         F  { 1, 2, 3 };

    struct_of_arrays<size_t>                         G  { {1}, {2}, {3} };
  }

  SECTION("assignment through iterator") {
    struct_of_arrays<size_t>                         E;
    struct_of_arrays<size_t, size_t>                 F;
    struct_of_arrays<size_t, size_t, double>         G;
    struct_of_arrays<double, size_t, size_t, double> H;

    E.push_back(8675309);
    F.push_back(867, 5309);
    G.push_back(867, 5309, 3.14159);
    H.push_back(95141.3, 867, 5309, 3.14159);
    H.push_back(195141.3, 1867, 15309, 13.14159);
    H.push_back(295141.3, 2867, 25309, 23.14159);
    H.push_back(395141.3, 3867, 35309, 33.14159);

    auto e = E.begin();
    test_assignment<0>(e);
    auto f = F.begin();
    test_assignment<0>(f);
    test_assignment<1>(f);
    auto h = H.begin();
    test_assignment<0>(h);
    test_assignment<1>(h);
    test_assignment<3>(h);
    test_assignment<0>(h, 3);
    test_assignment<1>(h, 3);
    test_assignment<2>(h, 3);
    test_assignment<3>(h, 3);
  }

  SECTION("assignment through iterator") {
    
  }
}



