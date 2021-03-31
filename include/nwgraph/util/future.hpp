//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Hartmut Kaiser (c) 2021
//

#ifndef NW_GRAPH_FUTURE_HPP
#define NW_GRAPH_FUTURE_HPP

#if NW_GRAPH_NEED_HPX
#include <hpx/async.hpp>
#include <hpx/future.hpp>
#include <hpx/thread.hpp>

namespace nw {
namespace graph {
using hpx::async;
template<typename T>
using future = hpx::future<T>;
using hpx::thread;
namespace launch {
constexpr inline hpx::launch::async_policy async{};
}    // namespace launch
}    // namespace graph
}    // namespace nw
#else
#include <future>
#include <thread>

namespace nw {
namespace graph {
using std::async;
template<typename T>
using future = std::future<T>;
using std::thread;
namespace launch {
using std::launch::async;
}    // namespace launch
}    // namespace graph
}    // namespace nw
#endif

#endif
