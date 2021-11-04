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

#ifndef NW_GRAPH_ACCESS_HPP
#define NW_GRAPH_ACCESS_HPP

#include "util/tag_invoke.hpp"
#include <ranges>

#include "nwgraph/graph_base.hpp"

namespace nw {
namespace graph {

#define DECL_TAG_INVOKE(str)                                                                                                  \
  struct str##_tag final {                                                                                                    \
    inline constexpr auto operator()(auto&&... args) const noexcept(is_nothrow_tag_invocable_v<str##_tag, decltype(args)...>) \
        -> tag_invoke_result_t<str##_tag, decltype(args)...> {                                                                \
      return tag_invoke(*this, std::forward<decltype(args)>(args)...);                                                        \
    }                                                                                                                         \
  };                                                                                                                          \
  static inline constexpr str##_tag str {}

DECL_TAG_INVOKE(num_vertices);
DECL_TAG_INVOKE(num_edges);
DECL_TAG_INVOKE(degree);
DECL_TAG_INVOKE(source);
DECL_TAG_INVOKE(target);


}    // namespace graph
}    // namespace nw

#endif    // NW_GRAPH_ACCESS_HPP