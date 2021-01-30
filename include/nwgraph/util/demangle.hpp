// 
// This file is part of NW Graph (aka GraphPack) 
// (c) Pacific Northwest National Laboratory 2018-2021 
// (c) University of Washington 2018-2021 
// 
// Licensed under terms of include LICENSE file 
// 
// Authors: 
//     Andrew Lumsdaine	
//

#ifndef NW_GRAPH_DEMANGLE_HPP
#define NW_GRAPH_DEMANGLE_HPP

#if (defined(__GNUC__) || defined(__clang__)) && !defined(_WINDOWS)
#include <cxxabi.h>
#endif
#include <string>

namespace nw {
namespace graph {
#if (defined(__GNUC__) || defined(__clang__)) && !defined(_WINDOWS)
template<class... Args>
std::string demangle(Args&&... args) {
  auto        cstr = abi::__cxa_demangle(std::forward<Args>(args)...);
  std::string str(cstr);
  free(cstr);
  return str;
}
#else
template<class Arg0, class... Args>
std::string demangle(Arg0&& arg0, Args&&... args) {
  std::string str(arg0);
  return str;
}
#endif
}    // namespace graph
}    // namespace nw

#endif    // NW_GRAPH_DEMANGLE_HPP
