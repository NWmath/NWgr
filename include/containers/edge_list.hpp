//
// This file is part of BGL17 (aka NWGraph aka GraphPack aka the Graph Standard Library)
// (c) Pacific Northwest National Laboratory 2018-2020
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#ifndef NW_GRAPH_EDGE_LIST_HPP
#define NW_GRAPH_EDGE_LIST_HPP

#include "containers/soa.hpp"

#include "containers/compressed.hpp"
#include "graph_base.hpp"
#include "util/provenance.hpp"

#if defined(CL_SYCL_LANGUAGE_VERSION)
#include <dpstd/algorithm>
#include <dpstd/execution>
#include <dpstd/numeric>
#else
#include <execution>
#include <tbb/parallel_sort.h>
#endif

#if defined(BGL17_NEED_EXCLUSIVE_SCAN)
#include "detail/numeric.hpp"
#endif

#include "util/defaults.hpp"
#include "util/demangle.hpp"
#include "util/print_types.hpp"
#include "util/timer.hpp"
#include <algorithm>
#include <concepts>
#include <limits>
#include <numeric>
#include <tuple>

namespace nw {
namespace graph {

static bool g_debug_edge_list = false;
static bool g_time_edge_list  = false;

void debug_edge_list(bool flag = true) { g_debug_edge_list = flag; }

void time_edge_list(bool flag = true) { g_time_edge_list = flag; }

template <int idx, directedness sym, typename... Attributes>
class compressed_sparse;

template <int idx, typename... Attributes>
class adjacency;

template <int idx, succession cessor, typename... Attributes>
class packed;

template <typename... Attributes>
class adj_list;

template <std::unsigned_integral vertex_id_type, directedness edge_directedness = undirected, typename... Attributes>
class index_edge_list : public graph_base, public struct_of_arrays<vertex_id_type, vertex_id_type, Attributes...> {
public:
  using vertex_id_t = vertex_id_type;

private:
  using base    = struct_of_arrays<vertex_id_t, vertex_id_t, Attributes...>;
  using element = std::tuple<vertex_id_t, vertex_id_t, Attributes...>;

public:
  index_edge_list(size_t N) : graph_base(N){};

  index_edge_list() : graph_base(0) { open_for_push_back(); }

  index_edge_list(std::initializer_list<element> l) : graph_base(0) {
    open_for_push_back();

    for_each(l.begin(), l.end(), [&](element x) { push_back(x); });

    if (g_debug_edge_list) {
      std::cout << " max " << vertex_cardinality[0] << " " << vertex_cardinality[1] << std::endl;
    }

    close_for_push_back();

    if (g_debug_edge_list) {
      std::cout << " max " << vertex_cardinality[0] << " " << vertex_cardinality[1] << std::endl;
    }
  }

  template <directedness to_dir, int kdx = 0, class ExecutionPolicy = std::execution::parallel_unsequenced_policy>
  auto convert_directedness(ExecutionPolicy&& policy = {}) {
    if constexpr (edge_directedness == to_dir) {
      return index_edge_list(*this);
    } else if constexpr (edge_directedness == directed && to_dir == undirected) {

      index_edge_list<vertex_id_type, to_dir, Attributes...> x(graph_base::vertex_cardinality[0]);

      x.resize(base::size());
      x.open_for_push_back();
      x.vertex_cardinality[0] = vertex_cardinality[1];
      x.vertex_cardinality[1] = vertex_cardinality[0];

      std::copy(policy, base::begin(), base::end(), x.begin());
      const int jdx = (kdx + 1) % 2;

      std::for_each(policy, x.begin(), x.end(), [](auto&& f) {
        if (std::get<jdx>(f) < std::get<kdx>(f)) {
          std::swap(std::get<jdx>(f), std::get<kdx>(f));
        }
      });
      std::sort(policy, x.begin(), x.end(), [](const element& a, const element& b) -> bool {
        return std::tie(std::get<kdx>(a), std::get<jdx>(a)) < std::tie(std::get<kdx>(b), std::get<jdx>(b));
      });

      x.uniq();
      x.close_for_push_back();

      x.prv      = prv;
      int status = -4;
      x.prv.push_back(nw::graph::demangle(typeid(*this).name(), nullptr, nullptr, &status) + "::" + __func__,
                      std::string("to ") + nw::graph::demangle(typeid(to_dir).name(), nullptr, nullptr, &status) + " " +
                          std::to_string(to_dir));
      return x;
    } else if constexpr (edge_directedness == undirected && to_dir == directed) {

      index_edge_list<vertex_id_type, to_dir, Attributes...> x(2 * graph_base::vertex_cardinality[0]);
      x.open_for_push_back();

#if 1

      auto bb = this->begin();
      auto ee = this->end();

      std::for_each(bb, ee, [&](auto&& elt) {
        std::apply(
            [&](vertex_id_t& i, vertex_id_t& j, Attributes... attrs) {
              x.push_back(i, j, attrs...);
              x.push_back(j, i, attrs...);
            },
            elt);
      });

#else

      for (auto j : base::storage_) {
        x.push_back(j);
        std::swap(std::get<0>(j), std::get<1>(j));
        x.push_back(j);
      }
#endif

      int status = -4;
      x.close_for_push_back();
      x.prv = prv;
      x.prv.push_back(nw::graph::demangle(typeid(*this).name(), nullptr, nullptr, &status) + "::" + __func__,
                      std::string("to ") + nw::graph::demangle(typeid(to_dir).name(), nullptr, nullptr, &status) + " " +
                          std::to_string(to_dir));
      return x;
    }
    return index_edge_list<vertex_id_type, to_dir, Attributes...>(0);
  }

  void open_for_push_back() { is_open = true; }

  void close_for_push_back() {
    vertex_cardinality[0] = vertex_cardinality[0] + 1;
    vertex_cardinality[1] = vertex_cardinality[1] + 1;

    is_open = false;
  }

  void push_back(vertex_id_t i, vertex_id_t j, Attributes... attrs) {
    assert(is_open == true);

    vertex_cardinality[0] = std::max<vertex_id_t>(i, vertex_cardinality[0]);
    vertex_cardinality[1] = std::max<vertex_id_t>(j, vertex_cardinality[1]);

    base::push_back(i, j, attrs...);
  }

  void push_back(const element& elem) {
    vertex_id_t i = std::get<0>(elem);
    vertex_id_t j = std::get<1>(elem);

    vertex_cardinality[0] = std::max(i, vertex_cardinality[0]);
    vertex_cardinality[1] = std::max(j, vertex_cardinality[1]);

    base::push_back(elem);
  }

  template <int idx, class ExecutionPolicy = std::execution::parallel_unsequenced_policy>
  void sort_by(ExecutionPolicy&& policy = {}) {
    int status = -4;
    prv.push_back(nw::graph::demangle(typeid(*this).name(), nullptr, nullptr, &status) + "::" + __func__,
                  std::string("index ") + std::to_string(idx));

    std::sort(policy, base::begin(), base::end(),

              [](const element& a, const element& b) -> bool { return (std::get<idx>(a) < std::get<idx>(b)); });
  }

  template <int idx, class ExecutionPolicy = std::execution::parallel_unsequenced_policy>
  void stable_sort_by(ExecutionPolicy&& policy = {}) {
    int status = -4;
    prv.push_back(nw::graph::demangle(typeid(*this).name(), nullptr, nullptr, &status) + "::" + __func__,
                  std::string("index ") + std::to_string(idx));

    std::stable_sort(policy, base::begin(), base::end(),
                     [](const element& a, const element& b) -> bool { return (std::get<idx>(a) < std::get<idx>(b)); });
  }

  template <int idx, class ExecutionPolicy = std::execution::parallel_unsequenced_policy>
  void lexical_sort_by(ExecutionPolicy&& policy = {}) {
    int status = -4;
    prv.push_back(nw::graph::demangle(typeid(*this).name(), nullptr, nullptr, &status) + "::" + __func__,
                  std::string("index ") + std::to_string(idx));

    const int jdx = (idx + 1) % 2;

    //print_types(base::begin(), base::end());
    static_assert(std::is_same_v<decltype(base::begin()), typename base::iterator>);
    // static_assert(std::is_same_v<decltype(*base::begin()), typename base::value_type>);
    // print_types(*base::begin(), std::declval<typename base::iterator::reference>());

    if constexpr (idx == 0) {
      std::sort(policy, base::begin(), base::end()
                //            , [](auto&& a, auto&& b) -> bool { return a < b; }
      );
    } else {
      std::sort(policy, base::begin(), base::end(), [](const element& a, const element& b) -> bool {
        // print_types(a, b);
        return std::tie(std::get<1>(a), std::get<0>(a)) < std::tie(std::get<1>(b), std::get<0>(b));
      });
    }
  }

  template <int idx, class ExecutionPolicy = std::execution::parallel_unsequenced_policy>
  void lexical_stable_sort_by(ExecutionPolicy&& policy = {}) {
    int status = -4;
    prv.push_back(nw::graph::demangle(typeid(*this).name(), nullptr, nullptr, &status) + "::" + __func__,
                  std::string("index ") + std::to_string(idx));

    const int jdx = (idx + 1) % 2;

    std::stable_sort(policy, base::begin(), base::end(), [](const element& a, const element& b) -> bool {
      return std::tie(std::get<idx>(a), std::get<jdx>(a)) < std::tie(std::get<idx>(b), std::get<jdx>(b));
    });
  }

  template <int idx, directedness sym>
  void fill(compressed_sparse<idx, sym, Attributes...>& cs) {
    stable_sort_by<idx>();
    fill_sorted(cs);
  }

  template <int idx, directedness sym>
  void fill_sorted(compressed_sparse<idx, sym, Attributes...>& cs) {
    cs.open_for_push_back();

    std::for_each(base::begin(), base::end(), [&](auto&& elt) {
      std::apply([&](vertex_id_t i, vertex_id_t j, Attributes... attrs) { cs.push_back(i, j, attrs...); }, elt);
    });

    cs.close_for_push_back();
  }

  template <int idx, succession cessor>
  void fill(packed<idx, cessor, Attributes...>& cs) {
    if constexpr (edge_directedness == undirected) {

      swap_to_triangular<idx, cessor>();
      lexical_sort_by<idx>();

      cs.open_for_push_back();
      for (auto& elt : dynamic_cast<base>(*this)) {
        std::apply([&](vertex_id_t i, vertex_id_t j, Attributes... attrs) { cs.push_back(i, j, attrs...); }, elt);
      }
      cs.close_for_push_back();
    } else {
      std::cerr << "Edge list must be undirected" << std::endl;
    }
  }

#if defined(FILL_IF)
  template <int idx, succession cessor = successor>
  void fill_if(adjacency<idx, Attributes...>& cs) {
    if constexpr (cessor == predecessor) {
      fill_if<idx>(cs, std::less<vertex_id_t>{});
    } else if constexpr (cessor == successor) {
      fill_if<idx>(cs, std::greater<vertex_id_t>{});
    } else {
    }
  }

  template <int idx, typename Comparator = decltype(std::less<vertex_id_t>{}),
            class ExecutionPolicy = std::execution::parallel_unsequenced_policy>
  void fill_if(adjacency<idx, Attributes...>& cs, Comparator comp, ExecutionPolicy&& policy = {}) {
    const kdx = (idx + 1) % 2;

    if constexpr (edge_directedness == directed) {

      std::vector<index_t> degrees(vertex_cardinality + 1 + 1);
      std::for_each(base::begin(), base::end(),
                    [&](auto&& elt) { if (comp((std::get<idx>(elt), std::get<kdx>(elt))) { ++degrees[std::get<idx>(elt)]; } });

      exclusive_scan(/* std::execution::par, */ degrees.begin(), degrees.end(), degrees.begin(), (vertex_id_t)0);
      cs.indices_.resize(vertex_cardinality + 1 + 1);

      std::copy(policy, degrees.begin(), degrees.end(), cs.indices_.begin());
      cs.to_be_indexed_.resize(size());

      std::copy_if(policy, std::get<kdx>(base::storage_).begin(), std::get<kdx>(base::storage_).end(),
                   std::get<idx>(cs.to_be_indexed_.storage_).begin());
    }
  }
#endif    // FILL_IF

  template <int idx, class ExecutionPolicy = std::execution::parallel_unsequenced_policy, size_t... Is>
  void fill_helper(adjacency<idx, Attributes...>& cs, std::index_sequence<Is...> is, ExecutionPolicy&& policy = {}) {
    (..., (std::copy(policy, std::get<Is + 2>(dynamic_cast<base&>(*this)).begin(),
                     std::get<Is + 2>(dynamic_cast<base&>(*this)).end(), std::get<Is + 1>(cs.to_be_indexed_).begin())));
  }

  template <int idx, class T, class ExecutionPolicy = std::execution::parallel_unsequenced_policy, size_t... Is>
  void fill_helper(adjacency<idx, Attributes...>& cs, std::index_sequence<Is...> is, T& Tmp, ExecutionPolicy&& policy = {}) {
    (..., (std::copy(policy, std::get<Is + 2>(dynamic_cast<base&>(Tmp)).begin(), std::get<Is + 2>(dynamic_cast<base&>(Tmp)).end(),
                     std::get<Is + 1>(cs.to_be_indexed_).begin())));
  }

  template <int idx, class ExecutionPolicy = std::execution::parallel_unsequenced_policy>
  void fill(adjacency<idx, Attributes...>& cs, ExecutionPolicy&& policy = {}) {
    if constexpr (edge_directedness == directed) {

#if !defined(EDGELIST_AOS)
      sort_by<idx>(policy);
      auto degree = degrees<idx>();

      const int kdx = (idx + 1) % 2;
      cs.indices_.resize(std::max(vertex_cardinality[idx], vertex_cardinality[kdx]) + 1 + 1);
      //cs.indices_.resize(vertex_cardinality[idx] + 1 + 1);

      std::inclusive_scan(std::execution::par, degree.begin(), degree.end(), cs.indices_.begin() + 1);
      cs.to_be_indexed_.resize(base::to_be_indexed.size());

      //const int kdx = (idx + 1) % 2;
      std::copy(policy, std::get<kdx>(dynamic_cast<base&>(*this)).begin(), std::get<kdx>(dynamic_cast<base&>(*this)).end(),
                std::get<0>(cs.to_be_indexed_).begin());

      if constexpr (sizeof...(Attributes) > 0) {
        fill_helper<idx>(cs, std::make_integer_sequence<size_t, sizeof...(Attributes)>());
      }

#else    // IS AOS
      stable_sort_by<idx>(policy);
      cs.open_for_push_back();

      std::for_each(base::begin(), base::end(), [&](auto&& elt) {
        std::apply([&](vertex_id_t i, vertex_id_t j, Attributes... attrs) { cs.push_back(i, j, attrs...); }, elt);
      });
      cs.close_for_push_back();
#endif

    } else {    // undirected

      // index_edge_list<edge_directedness, Attributes...> Tmp(0); // BUG!!!!

      index_edge_list<vertex_id_type, directed, Attributes...> Tmp(
          0);    // undirected == directed with doubled (and swapped) edges

      Tmp.resize(2 * base::size());
      {
        auto _ = g_time_edge_list ? nw::util::life_timer(__func__ + std::string(" adj fill create Tmp")) : nw::util::empty_timer();

        std::copy(policy, base::begin(), base::end(), Tmp.begin());
        std::transform(policy, base::begin(), base::end(), Tmp.begin() + base::size(), [&](auto&& elt) {
          auto flt = elt;
          std::swap(std::get<0>(flt), std::get<1>(flt));
          return flt;
        });

        Tmp.vertex_cardinality[0] = Tmp.vertex_cardinality[1] = std::max(vertex_cardinality[0], vertex_cardinality[1]);
      }

      {
        auto _ = g_time_edge_list ? nw::util::life_timer(__func__ + std::string(" adj fill prepare cs")) : nw::util::empty_timer();

        // Tmp.stable_sort_by<idx>();  // may allocate extra memory

        Tmp.template sort_by<idx>();

        auto degree = Tmp.template degrees<idx>();    // Can have a fast version if we know it is sorted -- using equal_range
        cs.indices_.resize(Tmp.vertex_cardinality[idx] + 1 + 1);

        std::inclusive_scan(std::execution::par, degree.begin(), degree.end(), cs.indices_.begin() + 1);
        cs.to_be_indexed_.resize(Tmp.size());
      }
      {
        auto _ = g_time_edge_list ? nw::util::life_timer(__func__ + std::string(" adj fill copy to cs")) : nw::util::empty_timer();

        const int kdx = (idx + 1) % 2;
        std::copy(policy, std::get<kdx>(dynamic_cast<base&>(Tmp)).begin(), std::get<kdx>(dynamic_cast<base&>(Tmp)).end(),
                  std::get<0>(cs.to_be_indexed_).begin());

        if constexpr (sizeof...(Attributes) > 0) {
          fill_helper<idx>(cs, std::make_integer_sequence<size_t, sizeof...(Attributes)>(), Tmp);
        }
      }
    }
  }

  void fill(adj_list<Attributes...>& al) {
    if constexpr (edge_directedness == directed) {
      stable_sort_by<0>();
      al.open_for_push_back();

      std::for_each(base::begin(), base::end(), [&](auto&& elt) {
        std::apply([&](vertex_id_t i, vertex_id_t j, Attributes... attrs) { al.push_back(i, j, attrs...); }, elt);
      });

      al.close_for_push_back();

    } else {
      index_edge_list<vertex_id_type, edge_directedness, Attributes...> Tmp(0);
      Tmp.reserve(2 * base::size());
      Tmp.open_for_push_back();
      std::for_each(base::begin(), base::end(), [&](auto&& elt) {
        std::apply([&](vertex_id_t i, vertex_id_t j, Attributes... attrs) { Tmp.push_back(i, j, attrs...); }, elt);
        std::apply([&](vertex_id_t i, vertex_id_t j, Attributes... attrs) { Tmp.push_back(j, i, attrs...); }, elt);
      });
      Tmp.close_for_push_back();

      Tmp.stable_sort_by<0>();
      al.open_for_push_back();

      std::for_each(Tmp.begin(), Tmp.end(), [&](auto&& elt) {
        std::apply([&](vertex_id_t i, vertex_id_t j, Attributes... attrs) { al.push_back(i, j, attrs...); }, elt);
      });
      al.close_for_push_back();
    }
  }

  // size_t size() const { return base::size(); }
  // size_t length() const { return base::size(); }

  auto num_edges() const { return base::size(); }


  auto max() const { return vertex_cardinality; }

  void symmetrize_in_place() {
    int status = -4;
    prv.push_back(nw::graph::demangle(typeid(*this).name(), nullptr, nullptr, &status) + "::" + __func__);

    base::reserve(2 * base::size());
    for (auto j : base::storage_) {
      std::swap(std::get<0>(j), std::get<1>(j));
      base::push_back(j);
    }
    lexical_sort_by<0>();
    uniq();
  }

  template <int idx, succession cessor = predecessor>
  void _triangularize_() {
    int status = -4;
    prv.push_back(nw::graph::demangle(typeid(*this).name(), nullptr, nullptr, &status) + "::" + __func__,
                  std::string("index ") + std::to_string(idx) + " " +
                      nw::graph::demangle(typeid(cessor).name(), nullptr, nullptr, &status));

    if constexpr (edge_directedness == undirected) {
      if constexpr ((idx == 0 && cessor == predecessor) || (idx == 1 && cessor == successor)) {
        for (auto& f : base::storage_) {
          if (std::get<0>(f) < std::get<1>(f)) {
            std::swap(std::get<0>(f), std::get<1>(f));
          }
        }
      } else if constexpr ((idx == 0 && cessor == successor) || (idx == 1 && cessor == predecessor)) {
        for (auto& f : base::storage_) {
          if (std::get<1>(f) < std::get<0>(f)) {
            std::swap(std::get<1>(f), std::get<0>(f));
          }
        }
      }
    } else if constexpr (edge_directedness == directed) {
      if constexpr ((idx == 0 && cessor == predecessor) || (idx == 1 && cessor == successor)) {
        auto past_the_end = std::remove_if(base::begin(), base::end(), [](auto&& x) { return std::get<0>(x) < std::get<1>(x); });
        base::erase(past_the_end, base::end());
      } else if constexpr ((idx == 0 && cessor == successor) || (idx == 1 && cessor == predecessor)) {
        auto past_the_end = std::remove_if(base::begin(), base::end(), [](auto&& x) { return std::get<1>(x) < std::get<0>(x); });
        base::erase(past_the_end, base::end());
      }
    }
  }

  template <int idx>
  void swap_to_triangular(const std::string& cessor = "predecessor") {
    if (cessor == "predecessor") {
      swap_to_triangular<idx, predecessor>();
    } else if (cessor == "successor") {
      swap_to_triangular<idx, successor>();
    } else {
      std::cout << "Bad succession: " + cessor << std::endl;
    }
  }

  template <int idx, succession cessor = predecessor, class ExecutionPolicy = std::execution::parallel_unsequenced_policy>
  void swap_to_triangular(ExecutionPolicy&& policy = {}) {
    int status = -4;
    prv.push_back(nw::graph::demangle(typeid(*this).name(), nullptr, nullptr, &status) + "::" + __func__,
                  std::string("index ") + std::to_string(idx) + " " +
                      nw::graph::demangle(typeid(cessor).name(), nullptr, nullptr, &status));

    if constexpr ((idx == 0 && cessor == predecessor) || (idx == 1 && cessor == successor)) {
      std::for_each(policy, base::begin(), base::end(), [](auto&& f) {
        if (std::get<0>(f) < std::get<1>(f)) {
          std::swap(std::get<0>(f), std::get<1>(f));
        }
      });
    } else if constexpr ((idx == 0 && cessor == successor) || (idx == 1 && cessor == predecessor)) {
      std::for_each(policy, base::begin(), base::end(), [](auto&& f) {
        if (std::get<1>(f) < std::get<0>(f)) {
          std::swap(std::get<1>(f), std::get<0>(f));
        }
      });
    }
  }

  template <int idx, succession cessor = predecessor, class ExecutionPolicy = std::execution::parallel_unsequenced_policy>
  void filter_to_triangular(ExecutionPolicy&& policy = {}) {
    int status = -4;
    prv.push_back(nw::graph::demangle(typeid(*this).name(), nullptr, nullptr, &status) + "::" + __func__,
                  std::string("index ") + std::to_string(idx) + " " +
                      nw::graph::demangle(typeid(cessor).name(), nullptr, nullptr, &status));

    if constexpr ((idx == 0 && cessor == predecessor) || (idx == 1 && cessor == successor)) {
      auto past_the_end =
          std::remove_if(policy, base::begin(), base::end(), [](auto&& x) { return std::get<0>(x) < std::get<1>(x); });

      base::erase(past_the_end, base::end());
    } else if constexpr ((idx == 0 && cessor == successor) || (idx == 1 && cessor == predecessor)) {
      auto past_the_end =
          std::remove_if(policy, base::begin(), base::end(), [](auto&& x) { return std::get<1>(x) < std::get<0>(x); });
      base::erase(past_the_end, base::end());
    }
  }

  // Make entries unique -- in place -- remove adjacent redundancies
  // Requires entries to be sorted in both dimensions
  template <class ExecutionPolicy = std::execution::parallel_unsequenced_policy>
  void uniq(ExecutionPolicy&& policy = {}) {
    int status = -4;
    prv.push_back(nw::graph::demangle(typeid(*this).name(), nullptr, nullptr, &status) + "::" + __func__);

    auto past_the_end = std::unique(policy, base::begin(), base::end(), [](auto&& x, auto&& y) {
      return std::get<0>(x) == std::get<0>(y) && std::get<1>(x) == std::get<1>(y);
    });

    // base::erase(past_the_end, base::end());
    base::resize(past_the_end - base::begin());
  }

  void remove_self_loops() {
    int status = -4;
    prv.push_back(nw::graph::demangle(typeid(*this).name(), nullptr, nullptr, &status) + "::" + __func__);

    auto past_the_end = std::remove_if(/*std::execution::par_unseq,*/ base::begin(), base::end(),
                                       [](auto&& x) { return std::get<0>(x) == std::get<1>(x); });
    // base::erase(past_the_end, base::end());
    base::resize(past_the_end - base::begin());
  }

  template <int d_idx = 0, class ExecutionPolicy = std::execution::parallel_unsequenced_policy>
  auto degrees(ExecutionPolicy&& policy = {}) {
    std::vector<vertex_id_t> degree(vertex_cardinality[d_idx] + 1);

    if constexpr (edge_directedness == directed) {
      std::vector<std::atomic<vertex_id_t>> tmp(degree.size());
      std::for_each(policy, base::begin(), base::end(), [&](auto&& x) { ++tmp[std::get<d_idx>(x)]; });
      std::copy(policy, tmp.begin(), tmp.end(), degree.begin());
    } else if constexpr (edge_directedness == undirected) {
      std::vector<std::atomic<vertex_id_t>> tmp(degree.size());
      std::for_each(policy, base::begin(), base::end(), [&](auto&& x) {
        ++tmp[std::get<0>(x)];
        ++tmp[std::get<1>(x)];
      });
      std::copy(policy, tmp.begin(), tmp.end(), degree.begin());
    }
    return degree;
  }

  template <int idx = 0>
  auto perm_by_degree(std::string direction = "ascending") {
    int status = -4;
    prv.push_back(nw::graph::demangle(typeid(*this).name(), nullptr, nullptr, &status) + "::" + __func__,
                  "index " + std::to_string(idx) + " " + direction);

    auto degree = degrees<idx>();

    return perm_by_degree<idx>(degree, direction);
  }

  template <int idx = 0, class Vector, class ExecutionPolicy = std::execution::parallel_unsequenced_policy>
  auto perm_by_degree(Vector&& degree, std::string direction = "ascending", ExecutionPolicy&& policy = {}) {
    std::vector<vertex_id_t> perm(degree.size());

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

  template <class Vector = std::vector<vertex_id_t>, class ExecutionPolicy = std::execution::parallel_unsequenced_policy>
  void relabel(Vector&& perm, ExecutionPolicy&& policy = {}) {
    std::vector<vertex_id_t> iperm(perm.size());

    tbb::parallel_for(tbb::blocked_range(0ul, iperm.size()), [&](auto&& r) {
      for (auto i = r.begin(), e = r.end(); i != e; ++i) {
        iperm[perm[i]] = i;
      }
    });

    std::for_each(policy, base::begin(), base::end(), [&](auto&& x) {
      std::get<0>(x) = iperm[std::get<0>(x)];
      std::get<1>(x) = iperm[std::get<1>(x)];
    });
  }

  template <int idx, class Vector = std::vector<int>>
  void relabel_by_degree(std::string direction = "ascending", Vector&& degree = std::vector<int>(0)) {
    int status = -4;
    prv.push_back(nw::graph::demangle(typeid(*this).name(), nullptr, nullptr, &status) + "::" + __func__,
                  "index " + std::to_string(idx) + " " + direction);

    std::vector<vertex_id_t> perm = degree.size() == 0 ? perm_by_degree<0>(direction) : perm_by_degree<0>(degree, direction);
    relabel(perm);
  }

  constexpr static const char magic[27] = "NW Graph index_edge_list";

  void serialize(std::ostream& outfile) const {
    outfile.write(reinterpret_cast<const char*>(magic), sizeof(magic));
    size_t d = edge_directedness;
    outfile.write(reinterpret_cast<const char*>(&d), sizeof(d));
    outfile.write(reinterpret_cast<const char*>(vertex_cardinality), sizeof(vertex_cardinality));
    base::serialize(outfile);
  }

  void serialize(const std::string& filename) const {
    std::ofstream outfile(filename, std::ofstream::binary);
    serialize(outfile);
  }

  void deserialize(std::istream& infile) {
    char spell[sizeof(magic) + 1];
    infile.read(reinterpret_cast<char*>(spell), sizeof(magic));
    size_t d = edge_directedness;
    infile.read(reinterpret_cast<char*>(&d), sizeof(d));
    if (d != edge_directedness) {
      std::cout << std::string("Warning: Expected directedness ") + std::to_string(edge_directedness) + " but got " +
                       std::to_string(d)
                << std::endl;
    }
    infile.read(reinterpret_cast<char*>(vertex_cardinality), sizeof(vertex_cardinality));
    base::deserialize(infile);
    close_for_push_back();
  }

  void deserialize(const std::string& filename) {
    set_origin(filename);
    std::ifstream infile(filename, std::ifstream::binary);
    deserialize(infile);
  }

  void set_origin(const std::string& origin) { prv.push_back("Origin: " + origin); }

  void stream_prv(std::ostream& os = std::cout) const { prv.stream(os); }

  void stream_stats(std::ostream& os = std::cout) const {
    int status = -4;
    std::cout << "% ";
    std::cout << nw::graph::demangle(typeid(*this).name(), nullptr, nullptr, &status) + ": " +
                     "vertex_cardinality = " + std::to_string(graph_base::vertex_cardinality[0]) + " ";
    std::cout << std::string("base::size() = ") + std::to_string(base::size());
    std::cout << std::endl;
  }

  void stream_edges(std::ostream& os = std::cout) {
    std::for_each(base::begin(), base::end(), [](auto&& x) {
      std::apply([&](auto&... field) { ((std::cout << field << " "), ...); }, x);
      std::cout << std::endl;
    });
  }

  void stream(std::ostream& os = std::cout) { stream_edges(os); }

  bool operator==(index_edge_list<vertex_id_type, edge_directedness, Attributes...>& e) {
    return vertex_cardinality == e.vertex_cardinality && base::operator==(e);    //*this == e;
  }

  bool operator!=(index_edge_list<vertex_id_type, edge_directedness, Attributes...>& e) { return !operator==(e); }

public:
  provenance prv;
};

template <directedness edge_directedness = undirected, typename... Attributes>
using edge_list = index_edge_list<default_vertex_id_t, edge_directedness, Attributes...>;

}    // namespace graph
}    // namespace nw

#endif    // NW_GRAPH_EDGE_LIST_HPP
