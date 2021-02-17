// 
// This file is part of NW Graph (aka GraphPack) 
// (c) Pacific Northwest National Laboratory 2018-2021 
// (c) University of Washington 2018-2021 
// 
// Licensed under terms of include LICENSE file 
// 
// Authors: 
//     Andrew Lumsdaine	
//     Luke D'Alessandro	
//


#ifndef NW_GRAPH_UTIL_ARROW_PROXY_HPP
#define NW_GRAPH_UTIL_ARROW_PROXY_HPP

namespace nw::graph {

template <class Reference>
struct arrow_proxy {
  Reference  r;
  Reference* operator->() { return &r; }
};

}    // namespace nw::graph


#endif // NW_GRAPH_UTIL_ARROW_PROXY_HPP
