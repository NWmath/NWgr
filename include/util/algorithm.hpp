

#ifndef NW_GRAPH_ALGORITHM_HPP
#define NW_GRAPH_ALGORITHM_HPP

#if defined(CL_SYCL_LANGUAGE_VERSION)
#include <dpstd/algorithm>
#include <dpstd/numeric>

namespace nw {
namespace graph {
using std::copy;
using std::copy_if;
using std::for_each;
using std::for_loop;
using std::remove_if;
using std::sort;
using std::stable_sort;
using std::transform;
using std::transform_reduce;
using std::unique;
}    // namespace graph
}    // namespace nw
#elif NW_GRAPH_NEED_TBB
#include <algorithm>
#include <tbb/parallel_sort.h>

namespace nw {
namespace graph {
using std::copy;
using std::copy_if;
using std::for_each;
using std::for_loop;
using std::remove_if;
using std::sort;
using std::stable_sort;
using std::transform;
using std::transform_reduce;
using std::unique;
}    // namespace graph
}    // namespace nw
#elif NW_GRAPH_NEED_HPX
#include <hpx/algorithm.hpp>

namespace nw {
namespace graph {
using hpx::copy;
using hpx::copy_if;
using hpx::for_each;
using hpx::for_loop;
using hpx::remove_if;
using hpx::sort;
using hpx::stable_sort;
using hpx::transform;
using hpx::transform_reduce;
using hpx::unique;
}    // namespace graph
}    // namespace nw
#else
#error "Unknown parallelization backend"
#endif

#endif
