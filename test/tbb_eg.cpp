//
// This file is part of Standard Graph Library (SGL)
// (c) Pacific Northwest National Laboratory 2018
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#include <iostream>
#include <tbb/tbb.h>
#include <tuple>

#if defined(CL_SYCL_LANGUAGE_VERSioN)
#include <dpstd/execution>
#include <dpstd/iterators.h>
template <class T>
using counting_iterator = dpstd::counting_iterator<T>;
#else
#include <execution>
#include <tbb/iterators.h>
template <class T>
using counting_iterator = tbb::counting_iterator<T>;
#endif

#include "adaptors/edge_range.hpp"
#include "adaptors/plain_range.hpp"
#include "containers/compressed.hpp"
#include "containers/edge_list.hpp"

using namespace nw::graph;
using namespace nw::util;

int main() {

  edge_list<directed> E_list{{3, 4}, {3, 6}, {4, 6}, {7, 8}, {9, 3}};
  adjacency<0>        A_0(E_list);

  std::for_each(edge_range(A_0).begin(), edge_range(A_0).end(),
                [&](auto&& x) { std::cout << std::get<0>(x) << " " << std::get<1>(x) << std::endl; });

  tbb::parallel_for(edge_range(A_0), [&](auto&& x) {
    //std::cout << std::get<0>(x) << " " << std::get<1>(x) << std::endl;

    //  std::cout << "x is " << x.size() << std::endl;
    std::for_each(x.begin(), x.end(), [&](auto&& x) {
      std::cout << "( " + std::to_string(std::get<0>(x)) + ", " + std::to_string(std::get<1>(x)) + " )\n";
    });
  });

  return 0;
}
