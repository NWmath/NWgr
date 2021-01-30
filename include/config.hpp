//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Hartmut Kaiser (c) 2020
//

#ifndef __CONFIG_HPP
#define __CONFIG_HPP

#if defined(NW_GRAPH_NEED_HPX)
#include <hpx/config.hpp>
#endif

#if defined(__GNUC__) || defined(__clang__)
#define NW_GRAPH_ATTRIBUTE_NOINLINE [[gnu::noinline]]
#else
#define NW_GRAPH_ATTRIBUTE_NOINLINE
#endif

#endif
