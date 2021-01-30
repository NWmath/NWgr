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

#ifndef NW_GRAPH_ATOMIC
#define NW_GRAPH_ATOMIC

#include "nwgraph/util/traits.hpp"
#include <atomic>
#include <cstddef>
#include <type_traits>

#if !defined(__GNUC__) && !defined(__clang__)
#include <cstdint>
#include <intrin.h>
#pragma intrinsic(_InterlockedExchangeAdd)
#pragma intrinsic(_InterlockedOr8, _InterlockedOr16, _InterlockedOr, _InterlockedOr64)
#pragma intrinsic(_InterlockedCompareExchange)

namespace nw {
namespace util {
namespace detail {
template<std::size_t Size>
struct interlocked;

template<>
struct interlocked<1> {
  template<typename T>
  constexpr static T call_or(T volatile* value, T mask) {
    return _InterlockedOr8(reinterpret_cast<std::int8_t volatile*>(value), static_cast<std::int8_t>(mask));
  }
};

template<>
struct interlocked<2> {
  template<typename T>
  constexpr static T call_or(T volatile* value, T mask) {
    return _InterlockedOr16(reinterpret_cast<std::int16_t volatile*>(value), static_cast<std::int16_t>(mask));
  }
};

template<>
struct interlocked<4> {
  template<typename T>
  constexpr static T call_or(T volatile* value, T mask) {
    return _InterlockedOr(reinterpret_cast<long volatile*>(value), static_cast<long>(mask));
  }
};

template<>
struct interlocked<8> {
  template<typename T>
  constexpr static T call_or(T volatile* value, T mask) {
    return _InterlockedOr64(reinterpret_cast<std::int64_t volatile*>(value), static_cast<std::int64_t>(mask));
  }
};
}    // namespace detail
}    // namespace util
}    // namespace nw
#endif

namespace nw {
namespace graph {
/// @returns            The loaded value.
template <std::memory_order order, class T>
constexpr auto load(T&& t) {
  if constexpr (is_atomic_v<std::decay_t<T>>) {
    return std::forward<T>(t).load(order);
  } else {
#if defined(__GNUC__) || defined(__clang__)
    std::decay_t<T> tt;
    __atomic_load(std::addressof(std::forward<T>(t)), std::addressof(tt), order);
    return tt;
#else
    auto tmp = std::forward<T>(t);
    detail::interlocked<sizeof(tmp)>::call_or(&tmp, std::decay_t<T>{0});
    return tmp;
#endif
  }
}

/// Atomic store operation.
///
/// This wraps the store member functions for atomics, and simulates atomic_ref
/// for non-atomic integral types using gcc's atomic builtins.
///
/// @tparam       order The memory order for the operation [default: release]
/// @tparam           T The type of the variable to store to.
/// @tparam           U The type of the value to store.
///
/// @param            t The variable to store to.
/// @param            u The value to store.
template <std::memory_order order, class T, class U>
constexpr void store(T&& t, U&& u) {
  if constexpr (is_atomic_v<std::decay_t<T>>) {
    std::forward<T>(t).store(std::forward<U>(u), order);
  } else {
#if defined(__GNUC__) || defined(__clang__)
    __atomic_store(std::addressof(std::forward<T>(t)), std::addressof(u), order);
#else
    auto tmp = std::decay_t<T>{0};
    detail::interlocked<sizeof(tmp)>::call_or(&tmp, std::forward<T>(t));
#endif
  }
}

/// CAS an atomic.
///
/// This makes it easy to use acquire-release semantics in a compare_exchange
/// operation. It wraps the compare exchange operation for atomics, and
/// simulates atomic_ref for non-atomic integral types using gcc's atomic
/// builtins.
///
/// @tparam      success The memory order for successful operations [default: acquire/release]
/// @tparam      failure The memory order for the read operation on failure [default: acquire]
/// @tparam            T The type of the variable to operate on.
/// @tparam            U The type of the expected value.
/// @tparam            V The type of the value to store.
///
/// @param             t The variable to store to.
/// @param[in/out]     u The expected value (updated on failure).
/// @param             v The new value.
///
/// @returns        true If the compare and exchange succeeded
///                false Otherwise (`u` is updated)
template <std::memory_order success = std::memory_order_acq_rel, std::memory_order failure = std::memory_order_acquire, class T, class U,
          class V>
constexpr bool cas(T&& t, U&& u, V&& v) {
  if constexpr (is_atomic_v<std::decay_t<T>>) {
    return std::forward<T>(t).compare_exchange_strong(std::forward<U>(u), std::forward<V>(v), success, failure);
  } else {
#if defined(__GNUC__) || defined(__clang__)
    return __atomic_compare_exchange(std::addressof(std::forward<T>(t)), std::addressof(std::forward<U>(u)),
                                     std::addressof(std::forward<V>(v)), false, success, failure);
#else
    auto compare_with = std::forward<U>(u);
    auto old_value    = _InterlockedCompareExchange(std::addressof(std::forward<T>(t)), std::forward<V>(v), compare_with);
    return compare_with == old_value;
#endif
  }
}

/// Load the value using acquire semantics.
///
/// @tparam           T The type of the variable to load from.
///
/// @param            t The variable to load from.
///
/// @return             The loaded value.
template <class T>
constexpr auto acquire(T&& t) {
  return load<std::memory_order_acquire>(std::forward<T>(t));
}

template <class T>
constexpr auto relaxed(T&& t) {
  return load<std::memory_order_relaxed>(std::forward<T>(t));
}

/// Store the value using release semantics.
///
/// @tparam           T The type of the variable to store to.
/// @tparam           U The type of the value to store.
///
/// @param            t The variable to store to.
/// @param            u The value to store.
template <class T, class U>
constexpr void release(T&& t, U&& u) {
  store<std::memory_order_release>(std::forward<T>(t), std::forward<U>(u));
}

template <class T, class U>
constexpr void relaxed(T&& t, U&& u) {
  store<std::memory_order_relaxed>(std::forward<T>(t), std::forward<U>(u));
}

/// Atomic fetch-and-add.
///
/// This provides a uniform atomic fetch-and-add API interface that works for
/// both atomic integer and floating point integral types, and simulates
/// atomic_ref for non-atomic integral types.
///
/// @tparam       order The memory order for the operation [default: acquire/release]
/// @tparam           T The type of the variable.
/// @tparam           U The type of the value to add.
///
/// @param            t The variable to modify.
/// @param            u The value to add.
///
/// @returns            The value of the variable prior to the add operation.
template <std::memory_order order = std::memory_order_acq_rel, class T, class U>
constexpr auto fetch_add(T&& t, U&& u) {
  if constexpr (is_atomic_v<std::decay_t<T>>) {
    if constexpr (std::is_floating_point_v<remove_atomic_t<std::decay_t<T>>>) {
      auto&& e = acquire(t);
      while (!cas<order>(std::forward<T>(t), e, e + u))
        ;
      return e;
    } else {
      return t.fetch_add(std::forward<U>(u), order);
    }
  } else {
    /// fallback to compiler atomics here... C++20 has atomic_ref.
    if constexpr (std::is_floating_point_v<std::decay_t<T>>) {
      auto e = acquire(std::forward<T>(t));
      for (auto f = e + u; !cas<order>(std::forward<T>(t), e, f); f = e + u)
        ;
      return e;
    } else {
#if defined(__GNUC__) || defined(__clang__)
      return __atomic_fetch_add(std::addressof(std::forward<T>(t)), std::forward<U>(u), order);
#else
      auto tmp = std::forward<T>(t);
      _InterlockedExchangeAdd(std::addressof(tmp), std::forward<U>(u));
      return tmp;
#endif
    }
  }
}

template <std::memory_order order = std::memory_order_acq_rel, class T, class U>
constexpr auto fetch_or(T&& t, U&& u) {
  static_assert(!std::is_floating_point_v<std::decay_t<T>>, "Logical fetch_or invalid for floating point types.");
  if constexpr (is_atomic_v<std::decay_t<T>>) {
    return std::forward<T>(t).fetch_or(std::forward<U>(u), order);
  } else {
#if defined(__GNUC__) || defined(__clang__)
    return __atomic_fetch_or(std::addressof(std::forward<T>(t)), std::forward<T>(u), order);
#else
    auto tmp = std::forward<T>(t);
    detail::interlocked<sizeof(tmp)>::call_or(&tmp, std::forward<T>(u));
    return tmp;
#endif
  }
}
}    // namespace graph
}    // namespace nw

#endif    // NW_GRAPH_ATOMIC_HPP
