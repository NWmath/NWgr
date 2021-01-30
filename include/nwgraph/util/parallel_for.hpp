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

#ifndef NW_GRAPH_PARALLEL_FOR_HPP
#define NW_GRAPH_PARALLEL_FOR_HPP

#include "nwgraph/util/traits.hpp"
#if NW_GRAPH_NEED_TBB
#include <tbb/parallel_for.h>
#include <tbb/parallel_reduce.h>
#endif
#if NW_GRAPH_NEED_HPX
#include <hpx/algorithm.hpp>
#include <hpx/execution.hpp>
#include <hpx/numeric.hpp>
#endif

namespace nw {
namespace graph {
/// @tparam          Op The type of the operator.
/// @tparam          It The type of the iterator.
///
/// @param           op The operator to evaluate.
/// @param            i The iterator to evaluate.
///
/// @returns         op(i) if `i` is an integral type
///       op(unpack(i)...) if `i` is a tuple type
/// parallel_for_inner(*i) otherwise
template <class Op, class It>
auto parallel_for_inner(Op&& op, It&& i) {
  if constexpr (is_tuple_v<std::decay_t<It>>) {
    return std::apply([&](auto&&... args) { return std::forward<Op>(op)(std::forward<decltype(args)>(args)...); }, std::forward<It>(i));
  }
#if NW_GRAPH_NEED_HPX
  else {
    return std::forward<Op>(op)(std::forward<It>(i));
  }
#else
  else if constexpr (std::is_integral_v<std::decay_t<It>>) {
    return std::forward<Op>(op)(std::forward<It>(i));
  } else {
    if constexpr (is_tuple_v<decltype(*std::forward<It>(i))>) {
      return parallel_for_inner(std::forward<Op>(op), *std::forward<It>(i));
    } else {
      return std::forward<Op>(op)(*std::forward<It>(i));
    }
  }
#endif
}

/// Apply an operator to a range sequentially.
///
/// @tparam       Range The type of the range.
/// @tparam          Op The type of the operator.
///
/// @param        range The range to process.
/// @param           op The operator to evaluate.
template <class Range, class Op>
void parallel_for_sequential(Range&& range, Op&& op) {
  for (auto &&i = range.begin(), e = range.end(); i != e; ++i) {
    parallel_for_inner(op, i);
  }
}

/// Apply an operator to a range sequentially, reducing the result.
///
/// @tparam       Range The type of the range.
/// @tparam          Op The type of the operator.
/// @tparam      Reduce The type of the reduction.
/// @tparam           T The reduced type.
///
/// @param        range The range to process.
/// @param           op The operator to evaluate.
/// @param       reduce The reduction.
/// @param         init The initial value for the reduction.
///
/// @returns            The result of `reduce(op(i), ...)` for all `i` in
///                     `range`.
template <class Range, class Op, class Reduce, class T>
auto parallel_for_sequential(Range&& range, Op&& op, Reduce&& reduce, T init) {
  for (auto &&i = range.begin(), e = range.end(); i != e; ++i) {
    init = reduce(init, parallel_for_inner(op, i));
  }
  return init;
}

/// BGL's parallel_for wrapper.
///
/// This performs a dynamic check on `range.is_divisible()` before calling into
/// the parallel library, which we have found to be important for
/// performance. If not `range.is_divisible()` then it will perform a
/// synchronous for loop.
///
/// @tparam       Range The type of the range.
/// @tparam          Op The type of the operator.
///
/// @param        range The range to process.
/// @param           op The operator to evaluate.
template <class Range, class Op>
void parallel_for(Range&& range, Op&& op) {
  if (range.is_divisible()) {
#if NW_GRAPH_NEED_TBB
    tbb::parallel_for(std::forward<Range>(range),
                      [&](auto&& sub) { parallel_for_sequential(std::forward<decltype(sub)>(sub), std::forward<Op>(op)); });
#endif
#if NW_GRAPH_NEED_HPX
    hpx::ranges::for_each(hpx::execution::par, std::forward<Range>(range), std::forward<Op>(op));
#endif
  } else {
#if NW_GRAPH_NEED_HPX
    hpx::ranges::for_each(std::forward<Range>(range), std::forward<Op>(op));
#else
    parallel_for_sequential(std::forward<Range>(range), std::forward<Op>(op));
#endif
  }
}

/// BGL's parallel_for wrapper that supports reductions.
///
/// This performs the designated reduction over the range. It will perform the
/// reduction in parallel if `range.is_divisible()`, otherwise it will perform a
/// synchronous sequential reduction.
///
/// @tparam       Range The type of the range.
/// @tparam          Op The type of the operator.
/// @tparam      Reduce The type of the reduction.
/// @tparam           T The reduced type.
///
/// @param        range The range to process.
/// @param           op The operator to evaluate.
/// @param       reduce The reduction.
/// @param         init The initial value for the reduction.
///
/// @returns            The result of `reduce(op(i), ...)` for all `i` in
///                     `range`.
template <class Range, class Op, class Reduce, class T>
auto parallel_for(Range&& range, Op&& op, Reduce&& reduce, T init) {
  if (range.is_divisible()) {
#if NW_GRAPH_NEED_TBB
    return tbb::parallel_reduce(
        std::forward<Range>(range), init,
        [&](auto&& sub, auto partial) { return parallel_for_sequential(std::forward<decltype(sub)>(sub), op, reduce, partial); }, reduce);
#endif
#if NW_GRAPH_NEED_HPX
    return hpx::ranges::transform_reduce(hpx::execution::par, std::forward<Range>(range), init, std::forward<Reduce>(reduce),
                                         [&](auto&& elem) { return parallel_for_inner(op, std::forward<decltype(elem)>(elem)); });
#endif
  } else {
#if NW_GRAPH_NEED_HPX
    return hpx::ranges::transform_reduce(std::forward<Range>(range), init, std::forward<Reduce>(reduce),
                                         [&](auto&& elem) { return parallel_for_inner(op, std::forward<decltype(elem)>(elem)); });
#else
    return parallel_for_sequential(std::forward<Range>(range), std::forward<Op>(op), std::forward<Reduce>(reduce), init);
#endif
  }
}
}    // namespace graph
}    // namespace nw

#endif    // NW_GRAPH_PARALLEL_FOR_HPP
