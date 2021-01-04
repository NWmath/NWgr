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
#include "graph_base.hpp"

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

template <std::unsigned_integral vertex_id_type, typename graph_base_t, directedness direct = undirected, typename... Attributes>
class index_edge_list : public graph_base_t, public struct_of_arrays<vertex_id_type, vertex_id_type, Attributes...> {
public:
  using vertex_id_t                           = vertex_id_type;
  static const directedness edge_directedness = direct;
  using attributes_t                          = std::tuple<Attributes...>;

  using my_type         = index_edge_list<vertex_id_type, graph_base_t, direct, Attributes...>;
  using directed_type   = index_edge_list<vertex_id_type, graph_base_t, directed, Attributes...>;
  using undirected_type = index_edge_list<vertex_id_type, graph_base_t, undirected, Attributes...>;

  // private:
  using graph_base = graph_base_t;
  using base       = struct_of_arrays<vertex_id_t, vertex_id_t, Attributes...>;
  using element    = std::tuple<vertex_id_t, vertex_id_t, Attributes...>;

public:
  constexpr static const bool is_unipartite = std::is_same<graph_base, unipartite_graph_base>::value;

  constexpr index_edge_list(const index_edge_list&)            = default;
  constexpr index_edge_list& operator=(const index_edge_list&) = default;
  constexpr index_edge_list(index_edge_list&&)                 = default;
  constexpr index_edge_list& operator=(index_edge_list&&)      = default;

  index_edge_list(size_t N = 0) requires(std::is_same<graph_base, unipartite_graph_base>::value) : graph_base(N) {
    open_for_push_back();
  }
  index_edge_list(size_t M = 0, size_t N = 0) requires(std::is_same<graph_base, bipartite_graph_base>::value) : graph_base(M, N) {
    open_for_push_back();
  }

  index_edge_list(std::initializer_list<element> l) {
    open_for_push_back();

    for_each(l.begin(), l.end(), [&](element x) { push_back(x); });

    if (g_debug_edge_list) {
      if constexpr (is_unipartite) {
        std::cout << " max " << graph_base::vertex_cardinality[0] << std::endl;
      } else {
        std::cout << " max " << graph_base::vertex_cardinality[0] << " " << graph_base::vertex_cardinality[1] << std::endl;
      }
    }

    close_for_push_back();

    if (g_debug_edge_list) {
      if constexpr (is_unipartite) {
        std::cout << " max " << graph_base::vertex_cardinality[0] << std::endl;
      } else {
        std::cout << " max " << graph_base::vertex_cardinality[0] << " " << graph_base::vertex_cardinality[1] << std::endl;
      }
    }
  }

  void open_for_push_back() { graph_base::is_open = true; }

  void close_for_push_back() {
    graph_base::vertex_cardinality[0] = graph_base::vertex_cardinality[0] + 1;

    if constexpr (false == is_unipartite) {
      graph_base::vertex_cardinality[1] = graph_base::vertex_cardinality[1] + 1;
    }

    graph_base::is_open = false;
  }

  void push_back(vertex_id_t i, vertex_id_t j, Attributes... attrs) {
    assert(graph_base::is_open == true);

    if constexpr (is_unipartite) {
      graph_base::vertex_cardinality[0] = std::max<vertex_id_t>(std::max(i, j), graph_base::vertex_cardinality[0]);
    } else {
      graph_base::vertex_cardinality[0] = std::max<vertex_id_t>(i, graph_base::vertex_cardinality[0]);
      graph_base::vertex_cardinality[1] = std::max<vertex_id_t>(j, graph_base::vertex_cardinality[1]);
    }

    base::push_back(i, j, attrs...);
  }

  void push_back(const element& elem) {
    vertex_id_t i = std::get<0>(elem);
    vertex_id_t j = std::get<1>(elem);

    if constexpr (is_unipartite) {
      graph_base::vertex_cardinality[0] = std::max(std::max(i, j), graph_base::vertex_cardinality[0]);
    } else {
      graph_base::vertex_cardinality[0] = std::max(i, graph_base::vertex_cardinality[0]);
      graph_base::vertex_cardinality[1] = std::max(j, graph_base::vertex_cardinality[1]);
    }

    base::push_back(elem);
  }

  size_t size() const { return base::size(); }
  // size_t length() const { return base::size(); }
  // auto max() const { return graph_base::vertex_cardinality; }

  auto num_edges() const { return base::size(); }
  auto num_vertices() const { return graph_base::vertex_cardinality; }

  constexpr static const char magic[27] = "NW Graph index_edge_list";

  void serialize(std::ostream& outfile) const {
    outfile.write(reinterpret_cast<const char*>(magic), sizeof(magic));
    size_t d = edge_directedness;
    outfile.write(reinterpret_cast<const char*>(&d), sizeof(d));
    outfile.write(reinterpret_cast<const char*>(graph_base::vertex_cardinality), sizeof(graph_base::vertex_cardinality));
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
    infile.read(reinterpret_cast<char*>(&graph_base::vertex_cardinality[0]), sizeof(graph_base::vertex_cardinality));
    base::deserialize(infile);
    close_for_push_back();
  }

  void deserialize(const std::string& filename) {
    std::ifstream infile(filename, std::ifstream::binary);
    deserialize(infile);
  }

  void stream_stats(std::ostream& os = std::cout) const {
    int status = -4;
    std::cout << "% ";
    std::cout << nw::graph::demangle(typeid(*this).name(), nullptr, nullptr, &status) + ": " +
                     "graph_base::vertex_cardinality = " + std::to_string(graph_base::vertex_cardinality[0]) + " ";
    if constexpr (false == is_unipartite) {
      std::cout << std::to_string(graph_base::vertex_cardinality[1]) + " ";
    }
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

  bool operator==(index_edge_list<vertex_id_type, graph_base_t, edge_directedness, Attributes...>& e) {
    return graph_base::vertex_cardinality == e.graph_base::vertex_cardinality && base::operator==(e);    //*this == e;
  }

  bool operator!=(index_edge_list<vertex_id_type, graph_base_t, edge_directedness, Attributes...>& e) { return !operator==(e); }
};

template <directedness edge_directedness = undirected, typename... Attributes>
using edge_list = index_edge_list<default_vertex_id_t, unipartite_graph_base, edge_directedness, Attributes...>;

template <directedness edge_directedness = undirected, typename... Attributes>
using bi_edge_list = index_edge_list<default_vertex_id_t, bipartite_graph_base, edge_directedness, Attributes...>;

}    // namespace graph
}    // namespace nw

#endif    // NW_GRAPH_EDGE_LIST_HPP
