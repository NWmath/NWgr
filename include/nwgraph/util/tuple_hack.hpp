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


#ifndef NWGRAPH_TUPLE_HACK_HPP
#define NWGRAPH_TUPLE_HACK_HPP

template <class... Ts, std::size_t... Is>
void swap(std::tuple<Ts&...>&& x, std::tuple<Ts&...>&& y, std::index_sequence<Is...>) {

  (std::swap(std::get<Is>(x), std::get<Is>(y)), ...);
}

template <class... Ts>
void swap(std::tuple<Ts&...>&& x, std::tuple<Ts&...>&& y) {
  
  swap(std::move(x), std::move(y), std::make_index_sequence<sizeof...(Ts)>());
}


#endif NWGRAPH_TUPLE_HACK_HPP
