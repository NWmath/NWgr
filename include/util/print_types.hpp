
#ifndef PRINT_TYPES_HPP
#define PRINT_TYPES_HPP

template <class... Ts>
struct print_types_t;

template <class... Ts>
constexpr auto print_types(Ts...) {
  return print_types_t<Ts...>{};
}

#endif    // PRINT_TYPES_HPP
