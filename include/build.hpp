//
// This file is part of BGL17 (aka NWGraph aka GraphPack aka the Graph Standard Library)
// (c) Pacific Northwest National Laboratory 2018-2020
// (c) University of Washington 2018-2020
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#ifndef NW_GRAPH_BUILD_HPP
#define NW_GRAPH_BUILD_HPP

#include "graph_base.hpp"

#include <algorithm>
#include <atomic>
#include <execution>
#include <iostream>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

namespace nw {
namespace graph {

using default_execution_policy = std::execution::parallel_unsequenced_policy;

template <int idx, class edge_list_t, class ExecutionPolicy = default_execution_policy>
void sort_by(edge_list_t& el, ExecutionPolicy&& policy = {}) {
  std::sort(std::execution::seq, el.begin(), el.end(),
            [](const auto& a, const auto& b) -> bool { return (std::get<idx>(a) < std::get<idx>(b)); });
}

template <int idx, class edge_list_t, class ExecutionPolicy = default_execution_policy>
void stable_sort_by(edge_list_t& el, ExecutionPolicy&& policy = {}) {
  std::stable_sort(policy, el.begin(), el.end(),
                   [](const auto& a, const auto& b) -> bool { return (std::get<idx>(a) < std::get<idx>(b)); });
}

template <int idx, class edge_list_t, class ExecutionPolicy = default_execution_policy>
void lexical_sort_by(edge_list_t& el, ExecutionPolicy&& policy = {}) {
  static_assert(std::is_same_v<decltype(el.begin()), typename edge_list_t::iterator>);

  const int jdx = (idx + 1) % 2;

  if constexpr (idx == 0) {
    std::sort(policy, el.begin(), el.end());
  } else {
    std::sort(policy, el.begin(), el.end(), [](const auto& a, const auto& b) -> bool {
      return std::tie(std::get<1>(a), std::get<0>(a)) < std::tie(std::get<1>(b), std::get<0>(b));
    });
  }
}

template <int idx, class edge_list_t, class ExecutionPolicy = default_execution_policy>
void lexical_stable_sort_by(edge_list_t& el, ExecutionPolicy&& policy = {}) {

  const int jdx = (idx + 1) % 2;

  std::stable_sort(policy, el.begin(), el.end(), [](const auto& a, const auto& b) -> bool {
    return std::tie(std::get<idx>(a), std::get<jdx>(a)) < std::tie(std::get<idx>(b), std::get<jdx>(b));
  });
}

#if 0
template <class edge_list_t, class adjacency_t>
void push_back_fill_x(edge_list_t& el, adjacency_t& cs) {
  cs.open_for_push_back();

  std::for_each(el.begin(), el.end(), [&](auto&& elt) { cs.push_back(elt); });

  cs.close_for_push_back();
}

#endif

template <class adjacency_t, typename... Ts>
auto push_back_fill_helper(adjacency_t& cs, std::tuple<Ts...> const& theTuple) {

  std::apply([&](Ts const&... args) { cs.push_back(args...); }, theTuple);
}

template <class edge_list_t, class adjacency_t>
void push_back_fill(edge_list_t& el, adjacency_t& cs) {
  cs.open_for_push_back();

  std::for_each(el.begin(), el.end(), [&](auto&& elt) { push_back_fill_helper(cs, elt); });

  cs.close_for_push_back();
}

template <class edge_list_t, class adjacency_t, class ExecutionPolicy = default_execution_policy, size_t... Is>
void fill_helper(edge_list_t& el, adjacency_t& cs, std::index_sequence<Is...> is, ExecutionPolicy&& policy = {}) {
  (..., (std::copy(policy, std::get<Is + 2>(dynamic_cast<edge_list_t::base&>(el)).begin(),
                   std::get<Is + 2>(dynamic_cast<edge_list_t::base&>(el)).end(), std::get<Is + 1>(cs.to_be_indexed_).begin())));
}

template <class edge_list_t, class adjacency_t, class T, class ExecutionPolicy = default_execution_policy, size_t... Is>
void fill_helper(edge_list_t& el, adjacency_t& cs, std::index_sequence<Is...> is, T& Tmp, ExecutionPolicy&& policy = {}) {
  (..., (std::copy(policy, std::get<Is + 2>(dynamic_cast<edge_list_t::base&>(Tmp)).begin(),
                   std::get<Is + 2>(dynamic_cast<edge_list_t::base&>(Tmp)).end(), std::get<Is + 1>(cs.to_be_indexed_).begin())));
}

template <int idx, class edge_list_t, class adjacency_t, class ExecutionPolicy = default_execution_policy>
void fill(edge_list_t& el, adjacency_t& cs, ExecutionPolicy&& policy = {}) {
  if constexpr (edge_list_t::edge_directedness == nw::graph::directedness::directed) {

    sort_by<idx>(el, policy);
    auto degree = degrees<idx>(el);

    // Compress idx
    if constexpr (edge_list_t::is_unipartite) {
      cs.indices_.resize(el.num_vertices()[0] + 1);
    } else {
      cs.indices_.resize(el.num_vertices()[idx] + 1);    // ???
    }

    std::inclusive_scan(policy, degree.begin(), degree.end(), cs.indices_.begin() + 1);
    cs.to_be_indexed_.resize(el.size());

    // Copy kdx (the other index)
    const int kdx = (idx + 1) % 2;
    std::copy(policy, std::get<kdx>(dynamic_cast<edge_list_t::base&>(el)).begin(),
              std::get<kdx>(dynamic_cast<edge_list_t::base&>(el)).end(), std::get<0>(cs.to_be_indexed_).begin());

    // Copy properties
    if constexpr (std::tuple_size<typename edge_list_t::attributes_t>::value > 0) {
      fill_helper(el, cs, std::make_integer_sequence<size_t, std::tuple_size<typename edge_list_t::attributes_t>::value>());
    }

  } else {    // undirected -- this cannot be a bipartite graph

    assert(edge_list_t::is_unipartite == true);

    typename edge_list_t::directed_type Tmp(
        el.num_vertices()[0]);    // directedness doesn't matter for the Tmp, so just use same type as el
                                  // EXCEPT -- degrees does something different if undirected
    Tmp.resize(2 * el.size());

    std::copy(policy, el.begin(), el.end(), Tmp.begin());

    std::transform(policy, el.begin(), el.end(), Tmp.begin() + el.size(), [&](auto&& elt) {
      auto flt = elt;
      std::swap(std::get<0>(flt), std::get<1>(flt));
      return flt;
    });

    sort_by<idx>(Tmp);    // stable_sort may allocate extra memory

    auto degree = degrees<idx>(Tmp);    // Can have a fast version if we know it is sorted -- using equal_range
    cs.indices_.resize(el.num_vertices()[0] + 1);

    std::inclusive_scan(policy, degree.begin(), degree.end(), cs.indices_.begin() + 1);
    cs.to_be_indexed_.resize(Tmp.size());

    const int kdx = (idx + 1) % 2;
    std::copy(policy, std::get<kdx>(dynamic_cast<edge_list_t::base&>(Tmp)).begin(),
              std::get<kdx>(dynamic_cast<edge_list_t::base&>(Tmp)).end(), std::get<0>(cs.to_be_indexed_).begin());

    if constexpr (std::tuple_size<typename edge_list_t::attributes_t>::value > 0) {
      fill_helper(el, cs, std::make_integer_sequence<size_t, std::tuple_size<typename edge_list_t::attributes_t>::value>(), Tmp);
    }
  }
}

template <int idx, class edge_list_t>
void swap_to_triangular(edge_list_t& el, const std::string& cessor = "predecessor") {
  if (cessor == "predecessor") {
    swap_to_triangular<idx, edge_list_t, succession::predecessor>(el);
  } else if (cessor == "successor") {
    swap_to_triangular<idx, edge_list_t, succession::successor>(el);
  } else {
    std::cout << "Bad succession: " + cessor << std::endl;
  }
}

template <int idx, class edge_list_t, succession cessor = succession::predecessor, class ExecutionPolicy = default_execution_policy>
void swap_to_triangular(edge_list_t& el, ExecutionPolicy&& policy = {}) {

  if constexpr ((idx == 0 && cessor == succession::predecessor) || (idx == 1 && cessor == succession::successor)) {
    std::for_each(policy, el.begin(), el.end(), [](auto&& f) {
      if (std::get<0>(f) < std::get<1>(f)) {
        std::swap(std::get<0>(f), std::get<1>(f));
      }
    });
  } else if constexpr ((idx == 0 && cessor == succession::successor) || (idx == 1 && cessor == succession::predecessor)) {
    std::for_each(policy, el.begin(), el.end(), [](auto&& f) {
      if (std::get<1>(f) < std::get<0>(f)) {
        std::swap(std::get<1>(f), std::get<0>(f));
      }
    });
  }
}

// Make entries unique -- in place -- remove adjacent redundancies
// Requires entries to be sorted in both dimensions
template <class edge_list_t, class ExecutionPolicy = default_execution_policy>
void uniq(edge_list_t& el, ExecutionPolicy&& policy = {}) {

  auto past_the_end = std::unique(policy, el.begin(), el.end(), [](auto&& x, auto&& y) {
    return std::get<0>(x) == std::get<0>(y) && std::get<1>(x) == std::get<1>(y);
  });

  // el.erase(past_the_end, el.end());
  el.resize(past_the_end - el.begin());
}

template <class edge_list_t>
void remove_self_loops(edge_list_t& el) {
  auto past_the_end = std::remove_if(/*std::execution::par_unseq,*/ el.begin(), el.end(),
                                     [](auto&& x) { return std::get<0>(x) == std::get<1>(x); });
  // el.erase(past_the_end, el.end());
  el.resize(past_the_end - el.begin());
}

template <int d_idx = 0, class edge_list_t, class ExecutionPolicy = default_execution_policy>
auto degrees(edge_list_t& el, ExecutionPolicy&& policy = {}) {

  size_t d_size = 0;
  if constexpr (edge_list_t::is_unipartite) {
    d_size = el.num_vertices()[0];
  } else {
    d_size = el.num_vertices()[d_idx];
  }

  std::vector<typename edge_list_t::vertex_id_type> degree(d_size);

  if constexpr (edge_list_t::edge_directedness == directedness::directed) {
    std::vector<std::atomic<typename edge_list_t::vertex_id_type>> tmp(degree.size());

    std::for_each(/* policy, */ el.begin(), el.end(), [&](auto&& x) { ++tmp[std::get<d_idx>(x)]; });

    std::copy(/* policy, */ tmp.begin(), tmp.end(), degree.begin());

  } else if constexpr (edge_list_t::edge_directedness == directedness::undirected) {
    std::vector<std::atomic<typename edge_list_t::vertex_id_type>> tmp(degree.size());

    std::for_each(/* policy, */ el.begin(), el.end(), [&](auto&& x) {
      ++tmp[std::get<0>(x)];
      ++tmp[std::get<1>(x)];
    });
    std::copy(/* policy, */ tmp.begin(), tmp.end(), degree.begin());
  }
  return degree;
}

template <int idx = 0, class edge_list_t>
auto perm_by_degree(edge_list_t& el, std::string direction = "ascending") {
  auto degree = degrees<idx>(el);
  return perm_by_degree<idx>(el, degree, direction);
}

template <int idx = 0, class edge_list_t, class Vector, class ExecutionPolicy = default_execution_policy>
auto perm_by_degree(edge_list_t& el, const Vector& degree, std::string direction = "ascending", ExecutionPolicy&& policy = {}) {

  std::vector<typename edge_list_t::vertex_id_type> perm(degree.size());

  tbb::parallel_for(tbb::blocked_range(0ul, perm.size()), [&](auto&& r) {
    for (auto i = r.begin(), e = r.end(); i != e; ++i) {
      perm[i] = i;
    }
  });

  auto d = degree.begin();

  if (direction == "descending") {
    std::sort(policy, perm.begin(), perm.end(), [&](auto a, auto b) { return d[a] > d[b]; });
  } else if (direction == "ascending") {
    std::sort(policy, perm.begin(), perm.end(), [&](auto a, auto b) { return d[a] < d[b]; });
  } else {
    std::cout << "Unknown direction: " << direction << std::endl;
  }

  return perm;
}

template <class edge_list_t, class Vector, class ExecutionPolicy = default_execution_policy>
void relabel(edge_list_t& el, const Vector& perm, ExecutionPolicy&& policy = {}) {
  std::vector<typename edge_list_t::vertex_id_type> iperm(perm.size());

  tbb::parallel_for(tbb::blocked_range(0ul, iperm.size()), [&](auto&& r) {
    for (auto i = r.begin(), e = r.end(); i != e; ++i) {
      iperm[perm[i]] = i;
    }
  });

  std::for_each(policy, el.begin(), el.end(), [&](auto&& x) {
    std::get<0>(x) = iperm[std::get<0>(x)];
    std::get<1>(x) = iperm[std::get<1>(x)];
  });
}

template <int idx, class edge_list_t, class Vector = std::vector<int>>
void relabel_by_degree(edge_list_t& el, std::string direction = "ascending", const Vector& degree = std::vector<int>(0)) {

  std::vector<typename edge_list_t::vertex_id_type> perm =
      degree.size() == 0 ? perm_by_degree<0>(el, direction) : perm_by_degree<0>(el, degree, direction);

  relabel(el, perm);
}

}    // namespace graph
}    // namespace nw
#endif    // NW_GRAPH_BUILD_HPP
