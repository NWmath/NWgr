//
// This file is part of NWGraph
// (c) Pacific Northwest National Laboratory 2018-2020
// (c) University of Washington 2018-2020
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

// !!! https://quuxplusone.github.io/blog/2019/02/06/arrow-proxy/ !!!

#ifndef NW_GRAPH_UTIL_ARROW_PROXY_HPP
#define NW_GRAPH_UTIL_ARROW_PROXY_HPP

namespace nw::graph
{
// https://quuxplusone.github.io/blog/2019/02/06/arrow-proxy/
template<class Reference>
struct arrow_proxy {
  Reference r;
  Reference *operator->() {
    return &r;
  }
};
}
#endif // NW_GRAPH_UTIL_ARROW_PROXY_HPP
