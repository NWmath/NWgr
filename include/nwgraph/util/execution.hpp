//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Hartmut Kaiser (c) 2021
//

#ifndef NW_GRAPH_EXECUTION_HPP
#define NW_GRAPH_EXECUTION_HPP

#if defined(CL_SYCL_LANGUAGE_VERSION)
#include <dpstd/execution>

namespace nw {
namespace graph {
namespace execution {
using std::execution::par;
using std::execution::par_unseq;
using std::execution::parallel_policy;
using std::execution::parallel_unsequenced_policy;
using std::execution::seq;
using std::execution::sequenced_policy;
}    // namespace execution
using std::is_execution_policy;
template<typename ExPolicy>
inline constexpr bool is_execution_policy_v = std::is_execution_policy<ExPolicy>::value;
}    // namespace graph
}    // namespace nw
#elif NW_GRAPH_NEED_TBB
#include <execution>

namespace nw {
namespace graph {
namespace execution {
using std::execution::par;
using std::execution::par_unseq;
using std::execution::parallel_policy;
using std::execution::parallel_unsequenced_policy;
using std::execution::seq;
using std::execution::sequenced_policy;
}    // namespace execution
using std::is_execution_policy;
template<typename ExPolicy>
inline constexpr bool is_execution_policy_v = std::is_execution_policy<ExPolicy>::value;
}    // namespace graph
}    // namespace nw
#elif NW_GRAPH_NEED_HPX
#include <hpx/execution.hpp>

namespace nw {
namespace graph {
namespace execution {
using hpx::execution::par;
using hpx::execution::par_unseq;
using hpx::execution::parallel_policy;
using hpx::execution::parallel_unsequenced_policy;
using hpx::execution::seq;
using hpx::execution::sequenced_policy;
}    // namespace execution
using hpx::is_execution_policy;
template<typename ExPolicy>
inline constexpr bool is_execution_policy_v = hpx::is_execution_policy<ExPolicy>::value;
}    // namespace graph
}    // namespace nw
#else
#error "Unknown parallelization backend"
#endif

#endif
