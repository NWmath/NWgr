//
// This file is part of BGL17 (aka NWGraph aka GraphPack aka the Graph Standard Library)
// (c) Pacific Northwest National Laboratory 2018
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#ifndef NW_GRAPH_GRAPH_BASE_HPP
#define NW_GRAPH_GRAPH_BASE_HPP

#include <cstddef>

namespace nw {
namespace graph {

enum directedness { undirected, directed };    // Really about packed / tri
enum view_shape {
  rectangular,
  lower_triangular,
  unit_lower_triangular,
  strict_lower_triangular,
  upper_triangular,
  unit_upper_triangular,
  strict_upper_triangular,
  row_vector,
  column_vector,
  unoriented_vector
};
enum succession { successor, predecessor };

template <directedness dir>
class other_direction {
public:
  const directedness direction;
};

template <>
class other_direction<undirected> {
public:
  const directedness direction = directed;
};

template <>
class other_direction<directed> {
public:
  const directedness direction = undirected;
};

class graph_base {
public:
  graph_base(size_t d0) : vertex_cardinality{d0, d0}, is_open(false) {}
  graph_base(size_t d0, size_t d1) : vertex_cardinality{d0, d1}, is_open(false) {}

protected:
  size_t vertex_cardinality[2];     // ordinal limits
  bool   is_open;    // can we mutate graph
};

template <directedness sym, typename... Attributes>
class sparse_aos;

template <int idx, directedness sym, typename... Attributes>
class compressed_sparse;

template <int idx, directedness sym, typename... Attributes>
class vov_sparse;

template <int idx, directedness sym, typename... Attributes>
class adj_sparse;

}    // namespace graph
}    // namespace nw
#endif    // NW_GRAPH_GRAPH_BASE_HPP
