//
// This file is part of BGL17 (aka NWGraph aka GraphPack aka the Graph Standard Library)
// (c) Pacific Northwest National Laboratory 2018
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//


#include <forward_list>
#include <list>
#include <tuple>
#include <vector>

#include "nwgraph/compat.hpp"
#include "nwgraph/containers/zip.hpp"
#include "nwgraph/algorithms/bfs.hpp"

#include "nwgraph/graph_concepts.hpp"

using namespace nw::graph;


using fot = std::forward_list<std::tuple<int,int>>;
using lot = std::list<std::tuple<int,int>>;
using vot = std::vector<std::tuple<int,int>>;

using tof = std::tuple<std::forward_list<int>,std::forward_list<int>>;
using tol = std::tuple<std::list<int>,std::list<int>>;
using tov = std::tuple<std::vector<int>,std::vector<int>>;

using vof = std::vector<std::forward_list<std::tuple<int>>>;
using vol = std::vector<std::list<std::tuple<int>>>;
using vov = std::vector<std::vector<std::tuple<int>>>;



static_assert(vertex_list_c<fot>);
static_assert(vertex_list_c<lot>);
static_assert(vertex_list_c<vot>);


static_assert(edge_list_c<fot>);
static_assert(edge_list_c<lot>);
static_assert(edge_list_c<vot>);


// Need a single iterator returning tuple
// static_assert(edge_list_c<tof>);
// static_assert(edge_list_c<tol>);
// static_assert(edge_list_c<tov>);


// Zip only defined for random access at the moment
// static_assert(edge_list_c<zipped<std::forward_list<int>,std::forward_list<int>>>);
// static_assert(edge_list_c<zipped<std::list<int>,std::list<int>>>);
static_assert(edge_list_c<zipped<std::vector<int>,std::vector<int>>>);


// static_assert(edge_list_graph<fot>); // Has neither num_edges nor num_vertices
// static_assert(edge_list_graph<lot>); // Has num_edges, does not have num_vertices
// static_assert(edge_list_graph<vot>); // Has num_edges, does not have num_vertices


static_assert(adjacency_graph<vof>);
static_assert(adjacency_graph<vol>);
static_assert(adjacency_graph<vov>);






int main() {

  bfs_v0(vov(), 0);
  bfs_v0(vol(), 0);
  bfs_v0(vof(), 0);

  return 0;
}