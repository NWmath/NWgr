//
// This file is part of BGL17 (aka NWGraph aka GraphPack aka the Graph Standard Library)
// (c) Pacific Northwest National Laboratory and University of Washington 2021
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#include "util/tag_invoke.hpp"
#include <iostream>
#include <ranges>
#include <string>
#include <vector>

#if 0
struct edges_t final {
  inline constexpr auto operator()(auto&& args...) const {
    return tag_invoke(*this, args);
  }
};
static inline constexpr edges_t edges{};

struct num_vertices_t final {
  inline constexpr auto operator()(auto&& args...) const {
    return tag_invoke(*this, args);
  }
};
static inline constexpr num_vertices_t num_vertices{};

struct edges_t final {
  inline constexpr auto operator()(auto&&... args) const
    noexcept(is_nothrow_tag_invocable_v<edges_t, decltype(args)...>)
    -> tag_invoke_result_t<edges_t, decltype(args)...> {
    return tag_invoke(*this, std::forward<decltype(args)>(args)...);
  }
};
static inline constexpr edges_t edges{};

#endif

#define DECL_TAG_INVOKE(str)                                                                                                \
  struct str##_t final {                                                                                                    \
    inline constexpr auto operator()(auto&&... args) const noexcept(is_nothrow_tag_invocable_v<str##_t, decltype(args)...>) \
        -> tag_invoke_result_t<str##_t, decltype(args)...> {                                                                \
      return tag_invoke(*this, std::forward<decltype(args)>(args)...);                                                      \
    }                                                                                                                       \
  };                                                                                                                        \
  static inline constexpr str##_t str {}

DECL_TAG_INVOKE(edges);
DECL_TAG_INVOKE(vertices);
DECL_TAG_INVOKE(num_vertices);

struct foo {
  void one(const std::string& msg) const { std::cout << "This is foo one: " << msg << std::endl; }
  void two() const { std::cout << "This is foo two" << std::endl; }
};

struct bar {
  void one(const std::string& msg) const { std::cout << "This is bar one: " << msg << std::endl; }
  void three() const { std::cout << "This is bar three" << std::endl; }
};

auto tag_invoke(const edges_t, const bar& b, const std::string& msg) { b.one(msg); }
auto tag_invoke(const edges_t, const foo& f, const std::string& msg) { f.one(msg); }

auto tag_invoke(const vertices_t, const foo& f) { f.two(); }
auto tag_invoke(const vertices_t, const bar& b) { b.three(); }

//template <typename T>
//auto tag_invoke(const num_vertices_t, const std::vector<T>& b) {
//  return b.size();
//}

template <std::ranges::sized_range T>
auto tag_invoke(const num_vertices_t, const T& b) {
  return b.size();
}

int main() {
  edges(foo(), "hello");
  edges(bar(), "world");
  vertices(foo());

  std::vector<int> a(10);
  std::cout << "num vertices a: " << num_vertices(a) << std::endl;
}
