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

#include <cstddef>
#include <tuple>
#include <utility>


namespace nw::graph {

template <class>
struct is_swappable : std::false_type { };

template <class T> requires(std::is_lvalue_reference_v<T>)
struct is_swappable<T> : std::true_type { };

template <class... Ts>
struct is_swappable<std::tuple<Ts...>> : std::conjunction<is_swappable<Ts>...> {};

} // namespace nw::graph

namespace std {

#if 1
template <class... Ts> requires (nw::graph::is_swappable<Ts>::value && ...)
void swap(std::tuple<Ts...>&& x, std::tuple<Ts...>&& y) 
{
    using std::swap;
    using std::get;
    [&]<std::size_t... i>(std::index_sequence<i...>) {
        (swap(get<i>(x), get<i>(y)), ...);
    }(std::make_index_sequence<sizeof...(Ts)>());
}
#else
template <class... Ts, std::size_t... Is>
void swap(std::tuple<Ts&...>&& x, std::tuple<Ts&...>&& y, std::index_sequence<Is...>) {

  (std::swap(std::get<Is>(x), std::get<Is>(y)), ...);
}

template <class... Ts>
void swap(std::tuple<Ts&...>&& x, std::tuple<Ts&...>&& y) {
  
  swap(std::move(x), std::move(y), std::make_index_sequence<sizeof...(Ts)>());
}
#endif
}

#endif // NWGRAPH_TUPLE_HACK_HPP
