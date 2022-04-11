// 
// This file is part of NW Graph (aka GraphPack) 
// (c) Pacific Northwest National Laboratory 2018-2021 
// (c) University of Washington 2018-2021 
// 
// Licensed under terms of include LICENSE file 
// 
// Authors: 
//     Andrew Lumsdaine	
//

#ifndef NW_GRAPH_GRAPH_BASE_HPP
#define NW_GRAPH_GRAPH_BASE_HPP

#include <array>
#include <cstddef>

namespace nw {
namespace graph {

enum class succession { successor, predecessor };
enum class directedness { undirected, directed };

template <succession success>
struct other_succession {
  const succession cessor;
};

template <>
struct other_succession<succession::successor> {
  const succession cessor = succession::predecessor;
};

template <>
struct other_succession<succession::predecessor> {
  const succession cessor = succession::successor;
};

template <directedness dir>
struct other_direction {
  const directedness direction;
};

template <>
struct other_direction<directedness::undirected> {
  const directedness direction = directedness::directed;
};

template <>
class other_direction<directedness::directed> {
public:
  const directedness direction = directedness::undirected;
};

class unipartite_graph_base {
public:
  using vertex_cardinality_t = std::array<size_t, 1>;

  unipartite_graph_base(size_t d0 = 0) : vertex_cardinality{d0}, is_open(false) {}

  // auto num_edges() { return num_edges_; }

protected:
  vertex_cardinality_t vertex_cardinality;
  bool                 is_open;    // can we mutate graph
};

class bipartite_graph_base {
public:
  using vertex_cardinality_t = std::array<size_t, 2>;

  bipartite_graph_base(size_t d0 = 0, size_t d1 = 0) : vertex_cardinality{d0, d1}, is_open(false) {}
  bipartite_graph_base(vertex_cardinality_t cardinality) : vertex_cardinality(cardinality), is_open(false) {}

  // auto num_edges() { return num_edges_; }

protected:
  vertex_cardinality_t vertex_cardinality;    // ordinal limits
  bool                 is_open;               // can we mutate graph
};

// In NON-unipartite_graph_base, is_unipartite<unipartite_graph_base> == false
template <typename Graph_base>
struct is_unipartite {
  constexpr static const bool value = false;
};

// for unipartite_graph_base, is_unipartite<unipartite_graph_base> == true
template <>
struct is_unipartite<unipartite_graph_base> {
  constexpr static const bool value = true;
};

}    // namespace graph
}    // namespace nw
#endif    // NW_GRAPH_GRAPH_BASE_HPP
