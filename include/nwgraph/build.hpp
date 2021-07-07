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

#ifndef NW_GRAPH_BUILD_HPP
#define NW_GRAPH_BUILD_HPP

#include "util/print_types.hpp"

#include "nwgraph/util/algorithm.hpp"
#include "nwgraph/util/execution.hpp"
#include "nwgraph/util/proxysort.hpp"

#include "nwgraph/graph_base.hpp"
#include "nwgraph/graph_traits.hpp"

#include <algorithm>
#include <atomic>
#include <execution>
#include <iostream>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>


#include "graph_concepts.hpp"


namespace nw {
namespace graph {

using default_execution_policy = nw::graph::execution::parallel_unsequenced_policy;

template <int idx, class edge_list_t, class ExecutionPolicy = default_execution_policy>
void sort_by(edge_list_t& el, ExecutionPolicy&& policy = {}) {
  nw::graph::sort(nw::graph::execution::seq, el.begin(), el.end(),
                  [](const auto& a, const auto& b) -> bool { return (std::get<idx>(a) < std::get<idx>(b)); });
}

template <int idx, class edge_list_t, class ExecutionPolicy = default_execution_policy>
void stable_sort_by(edge_list_t& el, ExecutionPolicy&& policy = {}) {
    nw::graph::stable_sort(policy, el.begin(), el.end(), [](const auto& a, const auto& b) -> bool { return (std::get<idx>(a) < std::get<idx>(b)); });
}

template <int idx, class edge_list_t, class ExecutionPolicy = default_execution_policy>
void lexical_sort_by(edge_list_t& el, ExecutionPolicy&& policy = {}) {
  static_assert(std::is_same_v<decltype(el.begin()), typename edge_list_t::iterator>);

  const int jdx = (idx + 1) % 2;

  if constexpr (idx == 0) {
    nw::graph::sort(policy, el.begin(), el.end());
  } else {
    nw::graph::sort(policy, el.begin(), el.end(), [](const auto& a, const auto& b) -> bool {
      return std::tie(std::get<1>(a), std::get<0>(a)) < std::tie(std::get<1>(b), std::get<0>(b));
    });
  }
}

template <int idx, class edge_list_t, class ExecutionPolicy = default_execution_policy>
void lexical_stable_sort_by(edge_list_t& el, ExecutionPolicy&& policy = {}) {

  const int jdx = (idx + 1) % 2;

  nw::graph::stable_sort(policy, el.begin(), el.end(), [](const auto& a, const auto& b) -> bool {
    return std::tie(std::get<idx>(a), std::get<jdx>(a)) < std::tie(std::get<idx>(b), std::get<jdx>(b));
  });
}


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

template <typename edge_list_t, typename adj_list_t>
auto fill_adj_list(edge_list_t& el, adj_list_t& al) {
  size_t num_edges = 0;

  al.open_for_push_back();

  std::for_each(el.begin(), el.end(), [&](auto elt) {
    push_back_fill_helper(al, elt);
    ++num_edges;
    if constexpr (edge_list_t::edge_directedness == directedness::undirected) {
      std::swap(std::get<0>(elt), std::get<1>(elt));
      push_back_fill_helper(al, elt);
      ++num_edges;
    }
  });

  al.close_for_push_back();

  return num_edges;
}


template <class Vector1, class Vector2, class Perm>
void permute(const Vector1& vec1, Vector2& vec2, const Perm& perm) {
#if NW_GRAPH_NEED_TBB
  tbb::parallel_for(tbb::blocked_range(0ul, perm.size()), [&](auto&& r) {
    for (auto i = r.begin(), e = r.end(); i != e; ++i) {
      vec2[i] = vec1[perm[i]];
    }
  });
#elif NW_GRAPH_NEED_HPX
  hpx::for_loop(0ul, perm.size(), [&](auto i) { vec2[i] = vec1[perm[i]]; });
#else
#error "Unknown parallelization backend"
#endif
}

template <class edge_list_t, class adjacency_t, class Perm, size_t... Is>
void permute_helper(edge_list_t& el, adjacency_t& cs, std::index_sequence<Is...> is, const Perm& perm) {
  (..., (permute(std::get<Is + 2>(dynamic_cast<typename edge_list_t::base&>(el)), std::get<Is + 1>(cs.to_be_indexed_), perm)));
}

template <class edge_list_t, class adjacency_t, class Perm, size_t... Is>
void permute_helper_all(edge_list_t& el, adjacency_t& cs, std::index_sequence<Is...> is, const Perm& perm) {
  (..., (permute(std::get<Is + 1>(dynamic_cast<typename edge_list_t::base&>(el)), std::get<Is>(cs.to_be_indexed_), perm)));
}



template <class edge_list_t, class adjacency_t, class T, class Perm, size_t... Is>
void permute_helper(edge_list_t& el, adjacency_t& cs, std::index_sequence<Is...> is, T& Tmp, Perm& perm) {
  (..., (permute(std::get<Is + 2>(dynamic_cast<typename edge_list_t::base&>(Tmp)),std::get<Is + 1>(cs.to_be_indexed_), perm)));
}


template <class edge_list_t, class adjacency_t, class ExecutionPolicy = default_execution_policy, size_t... Is>
void fill_helper(edge_list_t& el, adjacency_t& cs, std::index_sequence<Is...> is, ExecutionPolicy&& policy = {}) {
  (..., (nw::graph::copy(policy, std::get<Is + 2>(dynamic_cast<typename edge_list_t::base&>(el)).begin(),
                   std::get<Is + 2>(dynamic_cast<typename edge_list_t::base&>(el)).end(), std::get<Is + 1>(cs.to_be_indexed_).begin())));
}

template <class edge_list_t, class adjacency_t, class ExecutionPolicy = default_execution_policy, size_t... Is>
void copy_helper(edge_list_t& el, adjacency_t& cs, std::index_sequence<Is...> is, size_t offset, ExecutionPolicy&& policy = {}) {
  (..., (nw::graph::copy(policy, std::get<Is + 2>(dynamic_cast<typename edge_list_t::base&>(el)).begin(),
                   std::get<Is + 2>(dynamic_cast<typename edge_list_t::base&>(el)).end(), std::get<Is + 1>(cs.to_be_indexed_).begin()+offset)));
}

template <class edge_list_t, class adjacency_t, class T, class ExecutionPolicy = default_execution_policy, size_t... Is>
void fill_helper(edge_list_t& el, adjacency_t& cs, std::index_sequence<Is...> is, T& Tmp, ExecutionPolicy&& policy = {}) {
  (..., (nw::graph::copy(policy, std::get<Is + 2>(dynamic_cast<typename edge_list_t::base&>(Tmp)).begin(),
                   std::get<Is + 2>(dynamic_cast<typename edge_list_t::base&>(Tmp)).end(), std::get<Is + 1>(cs.to_be_indexed_).begin())));
}


template <int idx, class edge_list_t, class adjacency_t, class Int, class ExecutionPolicy = default_execution_policy>
auto fill_directed(edge_list_t& el, Int N, adjacency_t& cs, ExecutionPolicy&& policy = {}) {

  auto degree = degrees<idx>(el);

  if constexpr (is_unipartite<edge_list_t>::value) {  // Compress idx
    cs.indices_.resize(N + 1);
  } else {
    cs.indices_.resize(N + 1);
  }

  nw::graph::inclusive_scan(policy, degree.begin(), degree.end(), cs.indices_.begin() + 1);
  cs.to_be_indexed_.resize(el.size());

#if 1

  sort_by<idx>(el);  // Need to do this in a way that will let us have const el
                     // If not, we should steal (move) the vectors rather than copy

  // Copy kdx (the other index)
  const int kdx = (idx + 1) % 2;
  nw::graph::copy(policy, std::get<kdx>(dynamic_cast<typename edge_list_t::base&>(el)).begin(),
        std::get<kdx>(dynamic_cast<typename edge_list_t::base&>(el)).end(), std::get<0>(cs.to_be_indexed_).begin());

  // Copy properties
  if constexpr (std::tuple_size<typename edge_list_t::attributes_t>::value > 0) {
    fill_helper(el, cs, std::make_integer_sequence<size_t, std::tuple_size<typename edge_list_t::attributes_t>::value>());
  }

  // auto perm = proxysort(std::get<idx>(el));
  // permute(cs.to_be_indexed_, perm, policy);

#else

  // Better solution:
  //   Create tmp array
  //   Copy sorting index array to tmp array
  //   Copy other arrays to cs.to_be_indexed_
  //   Sort everything in to_be_indexed with tmp
  //     Need a custom swap -- pass in ?  ADL ?
  //

  using vertex_id_type = typename decltype(std::get<idx>(el))::value_type;  /* vertex_id_t<edge_list_t> */
  auto perm = nw::util::proxysort(std::get<idx>(el), std::less<vertex_id_type>());

  // Copy other (permuted) indices
  const int kdx = (idx + 1) % 2;
  permute(std::get<kdx>(dynamic_cast<typename edge_list_t::base&>(el)), std::get<0>(cs.to_be_indexed_), perm);

  // Copy (permuted) properties
  if constexpr (std::tuple_size<typename edge_list_t::attributes_t>::value > 0) {
    permute_helper(el, cs, std::make_integer_sequence<size_t, std::tuple_size<typename edge_list_t::attributes_t>::value>(), perm);
  }

#endif
}


template <int idx, class edge_list_t, class Int, class adjacency_t, class ExecutionPolicy = default_execution_policy>
auto fill_undirected(edge_list_t& el, Int N, adjacency_t& cs, ExecutionPolicy&& policy = {}) {
  assert(is_unipartite<edge_list_t>::value);


#if 0

  // Do same thing in fill_undirected
  //   Create tmp array -- 2X size
  //   Copy sorting index array and other array to tmp array
  //   Copy other index array and sorting index array to cs.to_be_indexed_
  //   Copy property arrays to cs.to_be_indexed_ 2X
  //   Sort everything in to_be_indexed with tmp
  //     Need a custom swap -- pass in ?  ADL ?


  using vertex_id_type = vertex_id_t<edge_list_t>;

  std::vector<vertex_id_type> Tmp(2*el.size());
  nw::graph::copy(policy, std::get<0>(el).begin(), std::get<0>(el).end(), Tmp.begin());
  nw::graph::copy(policy, std::get<1>(el).begin(), std::get<1>(el).end(), Tmp.begin()+el.size());

  const int kdx = (idx + 1) % 2;
  nw::graph::copy(policy, std::get<kdx>(el).begin(), std::get<kdx>(el).end(), std::get<0>(cs.to_be_indexed_).begin());
  nw::graph::copy(policy, std::get<idx>(el).begin(), std::get<idx>(el).end(), std::get<0>(cs.to_be_indexed_).begin()+el.size());

  if constexpr (std::tuple_size<typename edge_list_t::attributes_t>::value > 0) {
    copy_helper(el, cs, std::make_integer_sequence<size_t, std::tuple_size<typename edge_list_t::attributes_t>::value>(), 0);
  }
  if constexpr (std::tuple_size<typename edge_list_t::attributes_t>::value > 0) {
    copy_helper(el, cs, std::make_integer_sequence<size_t, std::tuple_size<typename edge_list_t::attributes_t>::value>(), el.size());
  }

  auto perm = proxysort(Tmp, std::less<vertex_id_type>());

  permute_helper_all(el, cs, std::make_integer_sequence<size_t, std::tuple_size<typename edge_list_t::attributes_t>::value>()+1, perm);


#else


  typename edge_list_t::directed_type Tmp(N);    // directedness doesn't matter for the Tmp, so just use same type as el
                                                 // EXCEPT -- degrees does something different if undirected

  Tmp.resize(2 * el.size());

  nw::graph::copy(policy, el.begin(), el.end(), Tmp.begin());

  nw::graph::transform(policy, el.begin(), el.end(), Tmp.begin() + el.size(), [&](auto&& elt) {
    auto flt = elt;
    std::swap(std::get<0>(flt), std::get<1>(flt));
    return flt;
  });

  sort_by<idx>(Tmp);    // stable_sort may allocate extra memory

  {
    auto degree = degrees<idx>(Tmp);    // Can have a fast version if we know it is sorted -- using equal_range
    cs.indices_.resize(N + 1);
    nw::graph::inclusive_scan(policy, degree.begin(), degree.end(), cs.indices_.begin() + 1);
  }

  cs.to_be_indexed_.resize(Tmp.size());

  const int kdx = (idx + 1) % 2;
  nw::graph::copy(policy, std::get<kdx>(dynamic_cast<typename edge_list_t::base&>(Tmp)).begin(),
        std::get<kdx>(dynamic_cast<typename edge_list_t::base&>(Tmp)).end(), std::get<0>(cs.to_be_indexed_).begin());

  if constexpr (std::tuple_size<typename edge_list_t::attributes_t>::value > 0) {
    fill_helper(el, cs, std::make_integer_sequence<size_t, std::tuple_size<typename edge_list_t::attributes_t>::value>(), Tmp);
  }
#endif
}


template <int idx, class edge_list_t, class adjacency_t, class ExecutionPolicy = default_execution_policy>
auto fill(edge_list_t& el, adjacency_t& cs, ExecutionPolicy&& policy = {}) {
  if constexpr (edge_list_t::edge_directedness == nw::graph::directedness::directed) {
    fill_directed<idx>(el, num_vertices(el), cs, policy);
  } else {    // undirected -- this cannot be a bipartite graph
    fill_undirected<idx>(el, num_vertices(el), cs, policy);
  }
  return cs.to_be_indexed_.size();
}

template <int idx, class edge_list_t, class adjacency_t, class ExecutionPolicy = default_execution_policy>
auto fill(edge_list_t& el, adjacency_t& cs, directedness dir, ExecutionPolicy&& policy = {}) {
  if (dir == nw::graph::directedness::directed) {
    fill_directed<idx>(el, num_vertices(el), cs, policy);
  } else {    // undirected -- this cannot be a bipartite graph
    fill_undirected<idx>(el, num_vertices(el), cs, policy);
  }
  return cs.to_be_indexed_.size();
}



template <int idx, class edge_list_t>
void swap_to_triangular(edge_list_t& el, succession cessor) {
  if (cessor == succession::predecessor) {
    swap_to_triangular<idx, edge_list_t, succession::predecessor>(el);
  } else if (cessor == succession::successor) {
    swap_to_triangular<idx, edge_list_t, succession::successor>(el);
  } else {
    std::cout << "Bad succession: " << std::endl;
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
    nw::graph::for_each(policy, el.begin(), el.end(), [](auto&& f) {
      if (std::get<0>(f) < std::get<1>(f)) {
        std::swap(std::get<0>(f), std::get<1>(f));
      }
    });
  } else if constexpr ((idx == 0 && cessor == succession::successor) || (idx == 1 && cessor == succession::predecessor)) {
    nw::graph::for_each(policy, el.begin(), el.end(), [](auto&& f) {
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

  auto past_the_end = nw::graph::unique(policy, el.begin(), el.end(), [](auto&& x, auto&& y) {
    return std::get<0>(x) == std::get<0>(y) && std::get<1>(x) == std::get<1>(y);
  });

  // el.erase(past_the_end, el.end());
  el.resize(past_the_end - el.begin());
}

template <class edge_list_t>
void remove_self_loops(edge_list_t& el) {
  auto past_the_end =
      std::remove_if(/*std::execution::par_unseq,*/ el.begin(), el.end(), [](auto&& x) { return std::get<0>(x) == std::get<1>(x); });
  // el.erase(past_the_end, el.end());
  el.resize(past_the_end - el.begin());
}


template<degree_enumerable_graph Graph, class ExecutionPolicy = default_execution_policy>
auto degrees(const Graph& graph, ExecutionPolicy&& policy = {}) {
  std::vector<vertex_id_t<Graph>> degree_v(num_vertices(graph));

#if NW_GRAPH_NEED_TBB
  tbb::parallel_for(tbb::blocked_range(0ul, degree_v.size()), [&](auto&& r) {
    for (auto i = r.begin(), e = r.end(); i != e; ++i) {
      degree_v[i] = degree(graph[i]);
    }
  });
#elif NW_GRAPH_NEED_HPX
  hpx::for_loop(0ul, degree_v.size(), [&](auto i) { degree_v[i] = degree(graph[i]); });
#else
#error "Unknown parallelization backend"
#endif
  return degree_v;
}


template <int d_idx = 0, class edge_list_t, class ExecutionPolicy = default_execution_policy>
auto degrees(edge_list_t& el, ExecutionPolicy&& policy = {}) requires(!degree_enumerable_graph<edge_list_t>) {

  size_t d_size = 0;
  if constexpr (is_unipartite<edge_list_t>::value) {
    d_size = num_vertices(el);
  } else {
    d_size = el.num_vertices()[d_idx];
  }

  using vertex_id_type = typename edge_list_t::vertex_id_type;

  std::vector<vertex_id_type> degree(d_size);

  if constexpr (edge_list_t::edge_directedness == directedness::directed) {
    std::vector<std::atomic<vertex_id_type>> tmp(degree.size());

    std::for_each(/* policy, */ el.begin(), el.end(), [&](auto&& x) { ++tmp[std::get<d_idx>(x)]; });

    std::copy(/* policy, */ tmp.begin(), tmp.end(), degree.begin());

  } else if constexpr (edge_list_t::edge_directedness == directedness::undirected) {
    std::vector<std::atomic<vertex_id_type>> tmp(degree.size());

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

#if NW_GRAPH_NEED_TBB
  tbb::parallel_for(tbb::blocked_range(0ul, perm.size()), [&](auto&& r) {
    for (auto i = r.begin(), e = r.end(); i != e; ++i) {
      perm[i] = i;
    }
  });
#elif NW_GRAPH_NEED_HPX
  hpx::for_loop(0ul, perm.size(), [&](auto i) { perm[i] = i; });
#else
#error "Unknown parallelization backend"
#endif

  auto d = degree.begin();

  if (direction == "descending") {
    nw::graph::sort(policy, perm.begin(), perm.end(), [&](auto a, auto b) { return d[a] > d[b]; });
  } else if (direction == "ascending") {
    nw::graph::sort(policy, perm.begin(), perm.end(), [&](auto a, auto b) { return d[a] < d[b]; });
  } else {
    std::cout << "Unknown direction: " << direction << std::endl;
  }

  return perm;
}

template <class edge_list_t, class Vector, class ExecutionPolicy = default_execution_policy>
void relabel(edge_list_t& el, const Vector& perm, ExecutionPolicy&& policy = {}) {
  std::vector<typename edge_list_t::vertex_id_type> iperm(perm.size());

#if NW_GRAPH_NEED_TBB
  tbb::parallel_for(tbb::blocked_range(0ul, iperm.size()), [&](auto&& r) {
    for (auto i = r.begin(), e = r.end(); i != e; ++i) {
      iperm[perm[i]] = i;
    }
  });
#elif NW_GRAPH_NEED_HPX
  hpx::for_loop(0ul, iperm.size(), [&](auto i) { iperm[perm[i]] = i; });
#else
#error "Unknown parallelization backend"
#endif

  nw::graph::for_each(policy, el.begin(), el.end(), [&](auto&& x) {
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
