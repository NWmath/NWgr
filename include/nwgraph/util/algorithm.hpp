//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Hartmut Kaiser (c) 2021
//

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
using std::inclusive_scan;
using std::remove_if;
using std::set_intersection;
using std::sort;
using std::stable_sort;
using std::transform;
using std::transform_reduce;
using std::unique;
}    // namespace graph
}    // namespace nw
#elif NW_GRAPH_NEED_TBB
#include <algorithm>
#include <numeric>
#include <tbb/parallel_sort.h>

namespace nw {
namespace graph {
using std::copy;
using std::copy_if;
using std::for_each;
using std::inclusive_scan;
using std::remove_if;
using std::set_intersection;
using std::sort;
using std::stable_sort;
using std::transform;
using std::transform_reduce;
using std::unique;
}    // namespace graph
}    // namespace nw
#elif NW_GRAPH_NEED_HPX
#include <hpx/algorithm.hpp>
#include <hpx/numeric.hpp>

namespace nw {
namespace graph {
using hpx::copy;
using hpx::copy_if;
using hpx::for_each;
using hpx::for_loop;
using hpx::inclusive_scan;
using hpx::remove_if;
using hpx::set_intersection;
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
