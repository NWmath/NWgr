//
// This file is part of NWGraph aka GraphPack aka the Graph Standard Library
// (c) Pacific Northwest National Laboratory and University of Washington 2018-2021
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//


#ifndef NW_GRAPH_GRAPH_TRAITS_HPP
#define NW_GRAPH_GRAPH_TRAITS_HPP

namespace nw {
namespace graph {

template <typename G>
struct graph_traits {

  using vertex_id_type    = typename G::vertex_id_type;
  using vertex_size_type  = typename G::vertex_id_type;
  using num_vertices_type = typename G::num_vertices_type;
  using num_edges_type    = typename G::num_edges_type;

  using outer_iterator = typename G::outer_iterator;
  using inner_iterator = typename G::inner_iterator;

  using const_outer_iterator = typename G::const_outer_iterator;
  using const_inner_iterator = typename G::const_inner_iterator;
};

template <typename G>
using vertex_id_t = typename graph_traits<G>::vertex_id_type;

template <typename G>
using vertex_size_t = typename graph_traits<G>::vertex_size_type;

template <typename G>
using num_vertices_t = typename graph_traits<G>::num_vertices_type;

template <typename G>
using num_edges_t = typename graph_traits<G>::num_edges_type;

template <typename G>
using outer_iterator_t = typename graph_traits<G>::outer_iterator;

template <typename G>
using inner_iterator_t = typename graph_traits<G>::inner_iterator;

template <typename G>
using const_outer_iterator_t = typename graph_traits<G>::const_outer_iterator;

template <typename G>
using const_inner_iterator_t = typename graph_traits<G>::const_inner_iterator;

}    // namespace graph
}    // namespace nw

#endif    // NW_GRAPH_GRAPH_TRAITS_HPP
