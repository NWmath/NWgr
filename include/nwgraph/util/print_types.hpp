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

#ifndef PRINT_TYPES_HPP
#define PRINT_TYPES_HPP

template <class... Ts>
struct print_types_t;

template <class... Ts>
constexpr auto print_types(Ts...) {
  return print_types_t<Ts...>{};
}

#endif // PRINT_TYPES_HPP
