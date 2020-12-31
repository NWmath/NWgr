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

#ifndef NW_GRAPH_GRAPH_BASE_HPP
#define NW_GRAPH_GRAPH_BASE_HPP

#include <array>
#include <cstddef>

namespace nw {
namespace graph {

enum succession { successor, predecessor };
enum directedness { undirected, directed };

template <succession success>
struct other_succession {
  const succession cessor;
};

template <>
struct other_succession<successor> {
  const succession cessor = predecessor;
};

template <>
struct other_succession<predecessor> {
  const succession cessor = successor;
};

template <directedness dir>
struct other_direction {
  const directedness direction;
};

template <>
struct other_direction<undirected> {
  const directedness direction = directed;
};

template <>
class other_direction<directed> {
public:
  const directedness direction = undirected;
};

class unipartite_graph_base {
public:
  unipartite_graph_base(size_t d0 = 0) : vertex_cardinality {d0}, is_open(false) {}

  // auto num_edges() { return num_edges_; }

protected:
  std::array<size_t, 1> vertex_cardinality;
  bool                  is_open;    // can we mutate graph
};

class bipartite_graph_base {
public:
  bipartite_graph_base(size_t d0 = 0, size_t d1 = 0) : vertex_cardinality{d0, d1}, is_open(false) {}

  // auto num_edges() { return num_edges_; }

protected:
  std::array<size_t, 2> vertex_cardinality;    // ordinal limits
  bool                  is_open;    // can we mutate graph
};




}    // namespace graph
}    // namespace nw
#endif    // NW_GRAPH_GRAPH_BASE_HPP
