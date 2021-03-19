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

#ifndef NW_GRAPH_MAKE_PRIORITY_QUEUE_HPP
#define NW_GRAPH_MAKE_PRIORITY_QUEUE_HPP

#include <queue>
#include <vector>

namespace nw {
namespace graph {
/// @returns            A `std::priority_queue` instantiated with the proper
///                     types.
template <class T, class Container, class Compare>
constexpr auto make_priority_queue(Compare&& compare) {
  using PQ = std::priority_queue<T, Container, std::decay_t<Compare>>;
  return PQ{std::forward<Compare>(compare)};
}

/// Create a priority queue adapter for a `std::vector<T>`.
///
/// This convenience utility forwards the `compare` object to
/// `nw::graph::make_priority_queue` while selecting a `std::vector<T>` as the
/// container type.
///
/// @tparam           T The type of the stored element.
/// @tparam     Compare The type of the comparison object.
///
/// @param      compare The comparison object to use.
///
/// @returns            A `std::priority_queue` wrapping a `std::vector<T>` that
///                     uses the passed `compare` operation.
template <class T, class Compare>
constexpr auto make_priority_queue(Compare&& compare) {
  return make_priority_queue<T, std::vector<T>>(std::forward<Compare>(compare));
}
}    // namespace graph
}    // namespace nw

#endif    // NW_GRAPH_MAKE_PRIORITY_QUEUE_HPP
