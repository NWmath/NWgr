//
// This file is part of NWGraph aka GraphPack aka the Graph Standard Library
// (c) Pacific Northwest National Laboratory and University of Washington 2021
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//


#ifndef NW_GRAPH_ACCESS_HPP
#define NW_GRAPH_ACCESS_HPP

#include "detail/tag_invoke.hpp"
#include <ranges>

#include "graph_base.hpp"

namespace nw {
  namespace graph {


#define DECL_TAG_INVOKE(str)						\
  struct str##_tag final {						\
    inline constexpr auto operator()(auto&&... args) const		\
      noexcept(is_nothrow_tag_invocable_v<str##_tag, decltype(args)...>) \
      -> tag_invoke_result_t<str##_tag, decltype(args)...> {		\
      return tag_invoke(*this, std::forward<decltype(args)>(args)...);	\
    }									\
  };									\
  static inline constexpr str##_tag str{}
    

DECL_TAG_INVOKE(num_vertices);
DECL_TAG_INVOKE(num_edges);


template <std::ranges::random_access_range T>
auto tag_invoke(const num_vertices_tag, const T& b) {
  return b.size();
}

  }
}


#endif // NW_GRAPH_ACCESS_HPP
