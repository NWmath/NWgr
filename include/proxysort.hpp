

#ifndef PROXYSORT_HPP
#define PROXYSORT_HPP

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <numeric>
#include <vector>

// template<typename ThingToSort, typename Comparator>
// void proxysort(const ThingToSort& x, std::vector<size_t>& perm) {
//   std::iota(perm.begin(), perm.end(), 0);
//  std::sort(perm.begin(), perm.end(), [&](auto a, auto b) { return x(a) > x(b); });
//}

template<typename ThingToSort, typename Comparator, typename IntT>
void proxysort(const ThingToSort& x, std::vector<IntT>& perm, Comparator comp = std::greater<IntT>()) {
  std::iota(perm.begin(), perm.end(), 0);
  std::sort(perm.begin(), perm.end(), [&](auto a, auto b) { return comp(x[a], x[b]); });
}

template<typename IntT = uint32_t, typename Comparator, typename ThingToSort>
auto proxysort(const ThingToSort& x, Comparator comp = std::greater<IntT>()) {
  std::vector<IntT> perm(x.size());
  proxysort(x, perm, comp);
  return perm;
}

#endif    // PROXYSORT_HPP
