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

#include "aos.hpp"
#include "soa.hpp"

#include "compressed.hpp"
#include "graph_base.hpp"
#include "plain_range.hpp"
#include "provenance.hpp"

#if defined(EXECUTION_POLICY)
#if defined(CL_SYCL_LANGUAGE_VERSION)
#include <dpstd/algorithm>
#include <dpstd/execution>
#include <dpstd/numeric>
#else
#include <execution>
#include <tbb/parallel_sort.h>
#endif
#endif

#if defined(BGL17_NEED_EXCLUSIVE_SCAN)
#include "detail/numeric.hpp"
#endif

#include "util/demangle.hpp"
#include "util/print_types.hpp"
#include "util/timer.hpp"
#include "util/types.hpp"
#include <algorithm>
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

template <directedness edge_directedness = undirected, typename... Attributes>

// #define EDGELIST_AOS

#if defined(EDGELIST_AOS)
class edge_list : public graph_base, public array_of_structs<vertex_id_t, vertex_id_t, Attributes...> {
  using base = array_of_structs<vertex_id_t, vertex_id_t, Attributes...>;
#else
class edge_list : public graph_base, public struct_of_arrays<vertex_id_t, vertex_id_t, Attributes...> {
  using base = struct_of_arrays<vertex_id_t, vertex_id_t, Attributes...>;

#endif

  using element = std::tuple<vertex_id_t, vertex_id_t, Attributes...>;

public:
  edge_list(size_t N)
      : graph_base(N), min_({std::numeric_limits<vertex_id_t>::max(), std::numeric_limits<vertex_id_t>::max()}), max_({0, 0}) {}

  edge_list(std::initializer_list<element> l)
      : graph_base(l.size()), min_({std::numeric_limits<vertex_id_t>::max(), std::numeric_limits<vertex_id_t>::max()}),
        max_({0, 0}) {
    open_for_push_back();

    for_each(l.begin(), l.end(), [&](element x) { push_back(x); });

    std::cout << "min " << min_[0] << " " << min_[1] << " "
              << " max " << max_[0] << " " << max_[1] << std::endl;

    close_for_push_back();

    std::cout << "min " << min_[0] << " " << min_[1] << " "
              << " max " << max_[0] << " " << max_[1] << std::endl;
  }

  edge_list()
      : graph_base(0), min_({std::numeric_limits<vertex_id_t>::max(), std::numeric_limits<vertex_id_t>::max()}), max_({0, 0}) {
    open_for_push_back();
  }

  //edge_list(const edge_list&) = default;

  //edge_list<directed, Attributes...> convert (const edge_list<undirected, Attributes...>&) ;
  //edge_list<undirected, Attributes...> convert(const edge_list<directed, Attributes...>&);
  template <directedness to_dir, int kdx = 0>
  auto convert_directedness() {
    if constexpr (edge_directedness == to_dir) {
      return edge_list(*this);
    } else if constexpr (edge_directedness == directed && to_dir == undirected) {

      // edge_list<to_dir, Attributes...> x(*this);

      edge_list<to_dir, Attributes...> x(graph_base::lim[0]);

      x.resize(base::size());
      x.open_for_push_back();
      x.max_[0] = max_[1];
      x.max_[1] = max_[0];
      x.min_[0] = min_[1];
      x.min_[1] = min_[0];

#if defined(EXECUTION_POLICY)
      std::copy(std::execution::par_unseq, base::begin(), base::end(), x.begin());
      const int jdx = (kdx + 1) % 2;

      std::for_each(std::execution::par_unseq, x.begin(), x.end(), [](auto&& f) {
        if (std::get<jdx>(f) < std::get<kdx>(f)) {
          std::swap(std::get<jdx>(f), std::get<kdx>(f));
        }
      });
      std::sort(std::execution::par_unseq, x.begin(), x.end(), [](const element& a, const element& b) -> bool {
        return std::tie(std::get<kdx>(a), std::get<jdx>(a)) < std::tie(std::get<kdx>(b), std::get<jdx>(b));
      });

#else
      std::copy(base::begin(), base::end(), x.begin());
      const int jdx = (kdx + 1) % 2;

      std::for_each(x.begin(), x.end(), [](auto&& f) {
        if (std::get<jdx>(f) < std::get<kdx>(f)) {
          std::swap(std::get<jdx>(f), std::get<kdx>(f));
        }
      });
      std::sort(x.begin(), x.end(), [](const element& a, const element& b) -> bool {
        return std::tie(std::get<kdx>(a), std::get<jdx>(a)) < std::tie(std::get<kdx>(b), std::get<jdx>(b));
      });
#endif

      x.uniq();
      x.close_for_push_back();

      x.prv      = prv;
      int status = -4;
      x.prv.push_back(nw::graph::demangle(typeid(*this).name(), nullptr, nullptr, &status) + "::" + __func__,
                      std::string("to ") + nw::graph::demangle(typeid(to_dir).name(), nullptr, nullptr, &status) + " " +
                          std::to_string(to_dir));
      return x;
    } else if constexpr (edge_directedness == undirected && to_dir == directed) {

#if 0
      edge_list<to_dir, Attributes...> x(2*graph_base::lim[0]);
      x.open_for_push_back();

#else
      edge_list<to_dir, Attributes...> x(2 * graph_base::lim[0]);
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
#endif
      int status = -4;
      x.close_for_push_back();
      x.prv = prv;
      x.prv.push_back(nw::graph::demangle(typeid(*this).name(), nullptr, nullptr, &status) + "::" + __func__,
                      std::string("to ") + nw::graph::demangle(typeid(to_dir).name(), nullptr, nullptr, &status) + " " +
                          std::to_string(to_dir));
      return x;
    }
    return edge_list<to_dir, Attributes...>(0);
  }

  void open_for_push_back() {}

  void close_for_push_back() {

    if (min_[0] != 0 || min_[1] != 0) {
      vertex_id_t the_min = std::min(min_[0], min_[1]);

#if defined(EXECUTION_POLICY)
      std::for_each(std::execution::par, base::begin(), base::end(), [&](auto&& x) {
#else
      std::for_each(base::begin(), base::end(), [&](auto&& x) {
#endif
        std::get<0>(x) -= the_min;
        std::get<1>(x) -= the_min;
      });
      max_[0] -= the_min;
      min_[0] -= the_min;
      max_[1] -= the_min;
      min_[1] -= the_min;
    }

    lim[0] = max_[0] + 1;
    lim[1] = max_[1] + 1;
  }

  void push_back(vertex_id_t i, vertex_id_t j, Attributes... attrs) {
    min_[0] = std::min(i, min_[0]);
    min_[1] = std::min(j, min_[1]);

    max_[0] = std::max(i, max_[0]);
    max_[1] = std::max(j, max_[1]);

    base::push_back(i, j, attrs...);
  }

  void push_back(const element& elem) {
    vertex_id_t i = std::get<0>(elem);
    vertex_id_t j = std::get<1>(elem);

    min_[0] = std::min(i, min_[0]);
    min_[1] = std::min(j, min_[1]);

    max_[0] = std::max(i, max_[0]);
    max_[1] = std::max(j, max_[1]);

    base::push_back(elem);
  }

  template <int idx>
  void sort_by() {
    int status = -4;
    prv.push_back(nw::graph::demangle(typeid(*this).name(), nullptr, nullptr, &status) + "::" + __func__,
                  std::string("index ") + std::to_string(idx));

#if defined(EXECUTION_POLICY)
    std::sort(std::execution::par_unseq, base::begin(), base::end(),
#else
    std::sort(base::begin(), base::end(),
#endif
              [](const element& a, const element& b) -> bool { return (std::get<idx>(a) < std::get<idx>(b)); });
  }

  template <int idx>
  void stable_sort_by() {
    int status = -4;
    prv.push_back(nw::graph::demangle(typeid(*this).name(), nullptr, nullptr, &status) + "::" + __func__,
                  std::string("index ") + std::to_string(idx));

#if defined(EXECUTION_POLICY)
    std::stable_sort(std::execution::par_unseq, base::begin(), base::end(),
#else
    std::stable_sort(base::begin(), base::end(),
#endif
                     [](const element& a, const element& b) -> bool { return (std::get<idx>(a) < std::get<idx>(b)); });
  }

  template <int idx>
  void lexical_sort_by() {
    int status = -4;
    prv.push_back(nw::graph::demangle(typeid(*this).name(), nullptr, nullptr, &status) + "::" + __func__,
                  std::string("index ") + std::to_string(idx));

    const int jdx = (idx + 1) % 2;

    //print_types(base::begin(), base::end());
    static_assert(std::is_same_v<decltype(base::begin()), typename base::iterator>);
    // static_assert(std::is_same_v<decltype(*base::begin()), typename base::value_type>);
    // print_types(*base::begin(), std::declval<typename base::iterator::reference>());

    if constexpr (idx == 0) {
#if defined(EXECUTION_POLICY)
      std::sort(std::execution::par_unseq, base::begin(), base::end()
#else
      std::sort(base::begin(), base::end()
#endif
                //            , [](auto&& a, auto&& b) -> bool { return a < b; }
      );
    } else {
#if defined(EXECUTION_POLICY)
      std::sort(std::execution::par_unseq, base::begin(), base::end(),
#else
      std::sort(base::begin(), base::end(),
#endif
                [](const element& a, const element& b) -> bool {
                  // print_types(a, b);
                  return std::tie(std::get<1>(a), std::get<0>(a)) < std::tie(std::get<1>(b), std::get<0>(b));
                });
    }
  }

  template <int idx>
  void lexical_stable_sort_by() {
    int status = -4;
    prv.push_back(nw::graph::demangle(typeid(*this).name(), nullptr, nullptr, &status) + "::" + __func__,
                  std::string("index ") + std::to_string(idx));

    const int jdx = (idx + 1) % 2;

#if defined(EXECUTION_POLICY)
    std::stable_sort(std::execution::par_unseq, base::begin(), base::end(),
#else
    std::stable_sort(base::begin(), base::end(),
#endif
                     [](const element& a, const element& b) -> bool {
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

  template <int idx, typename Comparator = decltype(std::less<vertex_id_t>{})>
  void fill_if(adjacency<idx, Attributes...>& cs, Comparator comp) {
    const kdx = (idx + 1) % 2;

    if constexpr (edge_directedness == directed) {

      std::vector<index_t> degrees(max_ + 1 + 1);
      std::for_each(base::begin(), base::end(),
                    [&](auto&& elt) { if (comp((std::get<idx>(elt), std::get<kdx>(elt))) { ++degrees[std::get<idx>(elt)]; } });

#if defined(EXECUTION_POLICY)
      exclusive_scan(/* std::execution::par, */ degrees.begin(), degrees.end(), degrees.begin(), (vertex_id_t)0);
      cs.indices_.resize(max_ + 1 + 1);

      std::copy(std::execution::par_unseq, degrees.begin(), degrees.end(), cs.indices_.begin());
      cs.to_be_indexed_.resize(size());

      std::copy_if(std::execution::par_unseq, std::get<kdx>(base::storage_).begin(), std::get<kdx>(base::storage_).end(),
                   std::get<idx>(cs.to_be_indexed_.storage_).begin());
#else
      std::exclusive_scan(degrees.begin(), degrees.end(), degrees.begin(), (vertex_id_t)0);
      cs.indices_.resize(max_ + 1 + 1);
      std::copy(degrees.begin(), degrees.end(), cs.indices_.begin());

      auto foo = size();

      cs.to_be_indexed_.resize(size());

      std::copy(std::get<1>(base::storage_).begin(), std::get<1>(base::storage_).end(),
                std::get<0>(cs.to_be_indexed_.storage_).begin());

#endif    // EXECUTION_POLICY
    }
  }
#endif    // FILL_IF

  template <int idx, size_t... Is>
  void fill_helper(adjacency<idx, Attributes...>& cs, std::index_sequence<Is...> is) {
    (..., (
#if defined(EXECUTION_POLICY)
              std::copy(std::execution::par_unseq,
#else
              std::copy(
#endif    // defined(EXECUTION_POLICY)
                        std::get<Is + 2>(dynamic_cast<base&>(*this)).begin(), std::get<Is + 2>(dynamic_cast<base&>(*this)).end(),
                        std::get<Is + 1>(cs.to_be_indexed_).begin())));
  }

  template <int idx, class T, size_t... Is>
  void fill_helper(adjacency<idx, Attributes...>& cs, std::index_sequence<Is...> is, T& Tmp) {
    (..., (
              std::copy(std::execution::par_unseq,
                        std::get<Is + 2>(dynamic_cast<base&>(Tmp)).begin(), std::get<Is + 2>(dynamic_cast<base&>(Tmp)).end(),
                        std::get<Is + 1>(cs.to_be_indexed_).begin())));
  }


  template <int idx>
  void fill(adjacency<idx, Attributes...>& cs) {
    if constexpr (edge_directedness == directed) {

#if !defined(EDGELIST_AOS)
      stable_sort_by<idx>();
      auto degree = degrees<idx>();

      cs.indices_.resize(max_[idx] + 1 + 1);

      std::inclusive_scan(std::execution::par, degree.begin(), degree.end(), cs.indices_.begin() + 1);
      cs.to_be_indexed_.resize(size());

      const int kdx = (idx + 1) % 2;
      std::copy(std::execution::par_unseq, std::get<kdx>(dynamic_cast<base&>(*this)).begin(),
                std::get<kdx>(dynamic_cast<base&>(*this)).end(), std::get<0>(cs.to_be_indexed_).begin());

      if constexpr (sizeof...(Attributes) > 0) {
        fill_helper<idx>(cs, std::make_integer_sequence<size_t, sizeof...(Attributes)>());
      }

#else    // IS AOS
      stable_sort_by<idx>();
      cs.open_for_push_back();

      std::for_each(base::begin(), base::end(), [&](auto&& elt) {
        std::apply([&](vertex_id_t i, vertex_id_t j, Attributes... attrs) { cs.push_back(i, j, attrs...); }, elt);
      });
      cs.close_for_push_back();
#endif

    } else {    // undirected

      edge_list<edge_directedness, Attributes...> Tmp(0);
      Tmp.resize(2*base::size());
      {
        auto _ = g_time_edge_list ? nw::util::life_timer(__func__ + std::string(" adj fill create Tmp")) : nw::util::empty_timer();

        std::copy(std::execution::par_unseq, base::begin(), base::end(), Tmp.begin());
        std::transform(std::execution::par_unseq, base::begin(), base::end(), Tmp.begin()+base::size(), [&](auto&& elt) {
          auto flt = elt;
          std::swap(std::get<0>(flt), std::get<1>(flt));
          return flt;
        });

        Tmp.max_[0] = Tmp.max_[1] = std::max(max_[0], max_[1]);
        Tmp.min_[0] = Tmp.min_[1] = std::min(min_[0], min_[1]);
      }

      {
        auto _ = g_time_edge_list ? nw::util::life_timer(__func__ + std::string(" adj fill prepare cs")) : nw::util::empty_timer();
        // Tmp.stable_sort_by<idx>();
	Tmp.sort_by<idx>();
        auto degree = Tmp.degrees<idx>(); // Can have a fast version if we know it is sorted
        cs.indices_.resize(Tmp.max_[idx] + 1 + 1);

        std::inclusive_scan(std::execution::par, degree.begin(), degree.end(), cs.indices_.begin() + 1);
        cs.to_be_indexed_.resize(Tmp.size());
      }
      {
        auto _ = g_time_edge_list ? nw::util::life_timer(__func__ + std::string(" adj fill copy to cs")) : nw::util::empty_timer();


      const int kdx = (idx + 1) % 2;
      std::copy(std::execution::par_unseq, std::get<kdx>(dynamic_cast<base&>(Tmp)).begin(),
                std::get<kdx>(dynamic_cast<base&>(Tmp)).end(), std::get<0>(cs.to_be_indexed_).begin());

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
      edge_list<edge_directedness, Attributes...> Tmp(0);
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

  size_t size() const { return base::size(); }
  size_t length() const { return base::size(); }

  auto max() const { return max_; }

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

  template <int idx, succession cessor = predecessor>
  void swap_to_triangular() {
    int status = -4;
    prv.push_back(nw::graph::demangle(typeid(*this).name(), nullptr, nullptr, &status) + "::" + __func__,
                  std::string("index ") + std::to_string(idx) + " " +
                      nw::graph::demangle(typeid(cessor).name(), nullptr, nullptr, &status));

    if constexpr ((idx == 0 && cessor == predecessor) || (idx == 1 && cessor == successor)) {
#if defined(EXECUTION_POLICY)
      std::for_each(std::execution::par_unseq, base::begin(), base::end(),
#else
      std::for_each(base::begin(), base::end(),
#endif
                    [](auto&& f) {
                      if (std::get<0>(f) < std::get<1>(f)) {
                        std::swap(std::get<0>(f), std::get<1>(f));
                      }
                    });
    } else if constexpr ((idx == 0 && cessor == successor) || (idx == 1 && cessor == predecessor)) {
#if defined(EXECUTION_POLICY)
      std::for_each(std::execution::par_unseq, base::begin(), base::end(),
#else
      std::for_each(base::begin(), base::end(),
#endif
                    [](auto&& f) {
                      if (std::get<1>(f) < std::get<0>(f)) {
                        std::swap(std::get<1>(f), std::get<0>(f));
                      }
                    });
    }
  }

  template <int idx, succession cessor = predecessor>
  void filter_to_triangular() {
    int status = -4;
    prv.push_back(nw::graph::demangle(typeid(*this).name(), nullptr, nullptr, &status) + "::" + __func__,
                  std::string("index ") + std::to_string(idx) + " " +
                      nw::graph::demangle(typeid(cessor).name(), nullptr, nullptr, &status));

    if constexpr ((idx == 0 && cessor == predecessor) || (idx == 1 && cessor == successor)) {
#if defined(EXECUTION_POLICY)
      auto past_the_end = std::remove_if(std::execution::par_unseq, base::begin(), base::end(),
                                         [](auto&& x) { return std::get<0>(x) < std::get<1>(x); });

      base::erase(past_the_end, base::end());
    } else if constexpr ((idx == 0 && cessor == successor) || (idx == 1 && cessor == predecessor)) {
      auto past_the_end = std::remove_if(std::execution::par_unseq, base::begin(), base::end(),
                                         [](auto&& x) { return std::get<1>(x) < std::get<0>(x); });
      base::erase(past_the_end, base::end());
    }
#else
      auto past_the_end = std::remove_if(base::begin(), base::end(), [](auto&& x) { return std::get<0>(x) < std::get<1>(x); });
      base::erase(past_the_end, base::end());
    } else if constexpr ((idx == 0 && cessor == successor) || (idx == 1 && cessor == predecessor)) {
      auto past_the_end = std::remove_if(base::begin(), base::end(), [](auto&& x) { return std::get<1>(x) < std::get<0>(x); });
      base::erase(past_the_end, base::end());
    }
#endif
  }

  // Make entries unique -- in place -- remove adjacent redundancies
  // Requires entries to be sorted in both dimensions
  void uniq() {
    int status = -4;
    prv.push_back(nw::graph::demangle(typeid(*this).name(), nullptr, nullptr, &status) + "::" + __func__);

#if defined(EXECUTION_POLICY)
    auto past_the_end =
        std::unique(std::execution::par_unseq, base::begin(), base::end(),
#else
    auto past_the_end =
        std::unique(base::begin(), base::end(),
#endif
                    [](auto&& x, auto&& y) { return std::get<0>(x) == std::get<0>(y) && std::get<1>(x) == std::get<1>(y); });

    // base::erase(past_the_end, base::end());
    base::resize(past_the_end - base::begin());
  }

  void remove_self_loops() {
    int status = -4;
    prv.push_back(nw::graph::demangle(typeid(*this).name(), nullptr, nullptr, &status) + "::" + __func__);

#if defined(EXECUTION_POLICY)
    auto past_the_end = std::remove_if(/*std::execution::par_unseq,*/ base::begin(), base::end(),
#else
    auto past_the_end = std::remove_if(base::begin(), base::end(),
#endif
                                       [](auto&& x) { return std::get<0>(x) == std::get<1>(x); });
    // base::erase(past_the_end, base::end());
    base::resize(past_the_end - base::begin());
  }

  template <int d_idx = 0>
  auto degrees() {
    std::vector<index_t> degree(max_[d_idx] + 1);

#if defined(EXECUTION_POLICY)
    if constexpr (edge_directedness == directed) {
      std::vector<std::atomic<vertex_id_t>> tmp(degree.size());
      std::for_each(std::execution::par_unseq, base::begin(), base::end(), [&](auto&& x) { ++tmp[std::get<d_idx>(x)]; });
      std::copy(std::execution::par_unseq, tmp.begin(), tmp.end(), degree.begin());
    } else if constexpr (edge_directedness == undirected) {
      std::vector<std::atomic<vertex_id_t>> tmp(degree.size());
      std::for_each(std::execution::par_unseq, base::begin(), base::end(), [&](auto&& x) {
        ++tmp[std::get<0>(x)];
        ++tmp[std::get<1>(x)];
      });
      std::copy(std::execution::par_unseq, tmp.begin(), tmp.end(), degree.begin());
    }
#else
    if constexpr (edge_directedness == directed) {
      std::for_each(base::begin(), base::end(), [&](auto&& x) { ++degree[std::get<d_idx>(x)]; });
    } else if constexpr (edge_directedness == undirected) {
      const auto d_kdx = (d_idx + 1) % 2;
      std::for_each(base::begin(), base::end(), [&](auto&& x) {
        ++degree[std::get<d_idx>(x)];
        ++degree[std::get<d_kdx>(x)];
      });
    }
#endif
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

  template <int idx = 0, class Vector>
  auto perm_by_degree(Vector&& degree, std::string direction = "ascending") {
    std::vector<vertex_id_t> perm(degree.size());

#if defined(EXECUTION_POLICY)
    tbb::parallel_for(tbb::blocked_range(0ul, perm.size()), [&](auto&& r) {
      for (auto i = r.begin(), e = r.end(); i != e; ++i) {
        perm[i] = i;
      }
    });
#else
    std::iota(perm.begin(), perm.end(), 0);
#endif

    auto d = degree.begin();

#if defined(EXECUTION_POLICY)
    if (direction == "descending") {
      std::sort(std::execution::par_unseq, perm.begin(), perm.end(), [&](auto a, auto b) { return d[a] > d[b]; });
    } else if (direction == "ascending") {
      std::sort(std::execution::par_unseq, perm.begin(), perm.end(), [&](auto a, auto b) { return d[a] < d[b]; });
    }
#else
    if (direction == "descending") {
      std::sort(perm.begin(), perm.end(), [&](auto a, auto b) { return d[a] > d[b]; });
    } else if (direction == "ascending") {
      std::sort(perm.begin(), perm.end(), [&](auto a, auto b) { return d[a] < d[b]; });
    }
#endif
    else {
      std::cout << "Unknown direction: " << direction << std::endl;
    }

    return perm;
  }

  template <class Vector = std::vector<vertex_id_t>>
  void relabel(Vector&& perm) {
    std::vector<vertex_id_t> iperm(perm.size());

#if defined(EXECUTION_POLICY)
    tbb::parallel_for(tbb::blocked_range(0ul, iperm.size()), [&](auto&& r) {
      for (auto i = r.begin(), e = r.end(); i != e; ++i) {
        iperm[perm[i]] = i;
      }
    });
#else
    for (size_t j = 0; j < perm.size(); ++j) {    // FIXME!!!
      iperm[perm[j]] = j;
    }
#endif

#if defined(EXECUTION_POLICY)
    std::for_each(std::execution::par_unseq, base::begin(), base::end(),
#else
    std::for_each(base::begin(), base::end(),
#endif
                  [&](auto&& x) {
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

  constexpr static const char magic[16] = "BGL17 edge_list";

  void serialize(std::ostream& outfile) const {
    outfile.write(reinterpret_cast<const char*>(magic), sizeof(magic));
    size_t d = edge_directedness;
    outfile.write(reinterpret_cast<const char*>(&d), sizeof(d));
    outfile.write(reinterpret_cast<const char*>(lim), sizeof(lim));
    outfile.write(reinterpret_cast<const char*>(&min_), sizeof(min_));
    outfile.write(reinterpret_cast<const char*>(&max_), sizeof(max_));
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
    infile.read(reinterpret_cast<char*>(lim), sizeof(lim));
    infile.read(reinterpret_cast<char*>(&min_), sizeof(min_));
    infile.read(reinterpret_cast<char*>(&max_), sizeof(max_));
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
                     "lim = " + std::to_string(graph_base::lim[0]) + " ";
    std::cout << std::string("(min, max) = (") + std::to_string(min_[0]) + ", " + std::to_string(min_[1]) + ", " +
                     std::to_string(max_[0]) + ", " + std::to_string(max_[1]) + ")" + " ";
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

  bool operator==(edge_list<edge_directedness, Attributes...>& e) {
    return min_ == e.min_ && max_ == e.max_ && base::operator==(e);    //*this == e;
  }

  bool operator!=(edge_list<edge_directedness, Attributes...>& e) { return !operator==(e); }

public:
  provenance prv;

public:
  std::array<vertex_id_t, 2> min_, max_;
};

}    // namespace graph
}    // namespace nw

#endif    // NW_GRAPH_EDGE_LIST_HPP
