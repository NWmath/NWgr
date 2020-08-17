
#ifndef NW_GRAPH_MAKE_PRIORITY_QUEUE_HPP
#define NW_GRAPH_MAKE_PRIORITY_QUEUE_HPP

#include <queue>
#include <vector>

namespace nw {
namespace graph {
/// Create a priority queue adapter for the passed container type.
///
/// This utility makes it easier to specify a priority queue that uses a custom
/// Compare operation. See `std::priority_queue` for a detailed specification of
/// the concepts required by `T`, `Container`, and `Compare`.
///
/// @tparam           T The type of the stored element.
/// @tparam   Container The type of the underlying container.
/// @tparam     Compare The type of the comparison object.
///
/// @param      compare The comparison object to use.
///
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
/// `bgl17::make_priority_queue` while selecting a `std::vector<T>` as the
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
