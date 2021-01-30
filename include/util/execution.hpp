#ifndef NW_GRAPH_EXECUTION_HPP
#define NW_GRAPH_EXECUTION_HPP

#if defined(CL_SYCL_LANGUAGE_VERSION)
#include <dpstd/execution>

namespace nw {
namespace graph {
namespace execution {
using std::execution::sequenced_policy;
using std::execution::parallel_policy;
using std::execution::parallel_unsequenced_policy;
using std::execution::seq;
using std::execution::par;
using std::execution::par_unseq;
}    // namespace execution
}    // namespace graph
}    // namespace nw
#elif NW_GRAPH_NEED_TBB
#include <execution>

namespace nw {
namespace graph {
namespace execution {
using std::execution::sequenced_policy;
using std::execution::parallel_policy;
using std::execution::parallel_unsequenced_policy;
using std::execution::seq;
using std::execution::par;
using std::execution::par_unseq;
}    // namespace execution
}    // namespace graph
}    // namespace nw
#elif NW_GRAPH_NEED_HPX
#include <hpx/execution.hpp>

namespace nw {
namespace graph {
namespace execution {
using hpx::execution::sequenced_policy;
using hpx::execution::parallel_policy;
using hpx::execution::parallel_unsequenced_policy;
using hpx::execution::seq;
using hpx::execution::par;
using hpx::execution::par_unseq;
}    // namespace execution
}    // namespace graph
}    // namespace nw
#else
#error "Unknown parallelization backend"
#endif

#endif
