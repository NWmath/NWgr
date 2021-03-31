//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Hartmut Kaiser (c) 2021
//

#ifndef NW_GRAPH_COUNTING_ITERATOR_HPP
#define NW_GRAPH_COUNTING_ITERATOR_HPP

#if defined(CL_SYCL_LANGUAGE_VERSION)
#include <dpstd/iterators.h>
namespace nw {
namespace graph {
template<class T>
using counting_iterator = dpstd::counting_iterator<T>;
}
}    // namespace nw
#elif NW_GRAPH_NEED_TBB
#include <tbb/iterators.h>
namespace nw {
namespace graph {
template<class T>
using counting_iterator = tbb::counting_iterator<T>;
}
}    // namespace nw
#elif NW_GRAPH_NEED_HPX
#include <hpx/modules/iterator_support.hpp>
namespace nw {
namespace graph {
template<class T>
using counting_iterator = hpx::util::counting_iterator<T>;
}
}    // namespace nw
#else
#error "Unknown parallelization backend"
#endif

#endif
