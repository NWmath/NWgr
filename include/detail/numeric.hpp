#ifndef __BGL17_NUMERIC_HPP
#define __BGL17_NUMERIC_HPP

#include <functional>
#include <numeric>

namespace std {

#if defined(BGL17_NEED_EXCLUSIVE_SCAN)

template <class _InputIterator, class _OutputIterator, class _Tp, class _BinaryOp>
_OutputIterator exclusive_scan(_InputIterator __first, _InputIterator __last, _OutputIterator __result, _Tp __init, _BinaryOp __b) {
  if (__first != __last) {
    _Tp __saved = __init;
    do {
      __init    = __b(__init, *__first);
      *__result = __saved;
      __saved   = __init;
      ++__result;
    } while (++__first != __last);
  }
  return __result;
}

template <class _InputIterator, class _OutputIterator, class _Tp>
_OutputIterator exclusive_scan(_InputIterator __first, _InputIterator __last, _OutputIterator __result, _Tp __init) {
  return exclusive_scan(__first, __last, __result, __init, std::plus<>());
}

#endif

#if defined(BGL17_NEED_INCLUSIVE_SCAN)
template <typename _InputIterator, typename _OutputIterator, typename _BinaryOperation, typename _Tp>
_OutputIterator inclusive_scan(_InputIterator __first, _InputIterator __last, _OutputIterator __result, _BinaryOperation __binary_op,
                               _Tp __init) {
  for (; __first != __last; ++__first)
    *__result++ = __init = __binary_op(__init, *__first);
  return __result;
}

template <typename _InputIterator, typename _OutputIterator, typename _BinaryOperation>
_OutputIterator inclusive_scan(_InputIterator __first, _InputIterator __last, _OutputIterator __result, _BinaryOperation __binary_op) {
  if (__first != __last) {
    auto __init = *__first;
    *__result++ = __init;
    ++__first;
    if (__first != __last) __result = std::inclusive_scan(__first, __last, __result, __binary_op, std::move(__init));
  }
  return __result;
}

template <typename _InputIterator, typename _OutputIterator>
_OutputIterator inclusive_scan(_InputIterator __first, _InputIterator __last, _OutputIterator __result) {
  return std::inclusive_scan(__first, __last, __result, std::plus<>());
}
#endif

#if defined(BGL17_NEED_REDUCE)
template <typename _InputIterator, typename _Tp, typename _BinaryOperation>
_Tp reduce(_InputIterator __first, _InputIterator __last, _Tp __init, _BinaryOperation __binary_op) {
  using value_type = typename iterator_traits<_InputIterator>::value_type;
  static_assert(is_invocable_r_v<_Tp, _BinaryOperation&, _Tp&, _Tp&>);
  static_assert(is_convertible_v<value_type, _Tp>);

  if constexpr (std::is_same_v<std::iterator_traits<_InputIterator>::iterator_category, std::random_access_iterator_tag>) {
    while ((__last - __first) >= 4) {
      _Tp __v1 = __binary_op(__first[0], __first[1]);
      _Tp __v2 = __binary_op(__first[2], __first[3]);
      _Tp __v3 = __binary_op(__v1, __v2);
      __init   = __binary_op(__init, __v3);
      __first += 4;
    }
  }
  for (; __first != __last; ++__first)
    __init = __binary_op(__init, *__first);
  return __init;
}

template <typename _InputIterator, typename _Tp>
_Tp reduce(_InputIterator __first, _InputIterator __last, _Tp __init) {
  return std::reduce(__first, __last, std::move(__init), std::plus<>());
}

template <typename _InputIterator>
typename iterator_traits<_InputIterator>::value_type reduce(_InputIterator __first, _InputIterator __last) {
  using value_type = typename iterator_traits<_InputIterator>::value_type;
  return std::reduce(__first, __last, value_type{}, std::plus<>());
}
#endif

}    // namespace std

#endif
