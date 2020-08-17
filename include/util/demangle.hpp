#pragma once

#include <string>
#include <cxxabi.h>

namespace bgl17 {
/// A demangle wrapper that frees the returned string.
template <class... Args>
std::string demangle(Args&&... args) {
  auto cstr = abi::__cxa_demangle(std::forward<Args>(args)...);
  std::string str(cstr);
  free(cstr);
  return str;
}
}
