.. SPDX-FileCopyrightText: 2022 Battelle Memorial Institute
.. SPDX-FileCopyrightText: 2022 University of Washington
..
.. SPDX-License-Identifier: BSD-3-Clause

NWGraph API Reference
=====================

.. highlight:: c++


Concepts
--------

.. doxygenconcept:: nw::graph::graph

.. doxygenconcept:: nw::graph::adjacency_list_graph

.. doxygenconcept:: nw::graph::degree_enumerable_graph

.. doxygenconcept:: nw::graph::edge_list_graph



Graph Algorithms
----------------

.. doxygenfunction:: nw::graph::brandes_bc(const Graph& G, bool normalize = true)

.. doxygenfunction:: nw::graph::brandes_bc(const Graph& graph, const std::vector<typename Graph::vertex_id_type>& sources, size_t threads, OuterExecutionPolicy&& outer_policy = {}, InnerExecutionPolicy&& inner_policy = {}, bool normalize = true)

.. doxygenfunction:: nw::graph::bfs(const Graph& graph, vertex_id_t<Graph> root)

.. doxygenfunction:: nw::graph::bfs(const OutGraph& out_graph, const InGraph& in_graph, vertex_id_t<OutGraph> root, int num_bins = 32, int alpha = 15, int beta = 18)

.. doxygenfunction:: nw::graph::bc2_v5

.. doxygenfunction:: nw::graph::bk_maxflow

.. doxygenfunction:: nw::graph::afforest

.. doxygenfunction:: nw::graph::dag_based_mis

.. doxygenfunction:: nw::graph::delta_stepping_v0

.. doxygenfunction:: nw::graph::delta_stepping_v12

.. doxygenfunction:: nw::graph::dijkstra

.. doxygenfunction:: nw::graph::dijkstra_v0

.. doxygenfunction:: nw::graph::jaccard_similarity_v0

.. doxygenfunction:: nw::graph::jones_plassmann_coloring

.. doxygenfunction:: nw::graph::k_core

.. doxygenfunction:: nw::graph::kruskal(EdgeListT &E, Compare comp)

.. doxygenfunction:: nw::graph::kruskal(EdgeListT &E)

.. doxygenfunction:: nw::graph::max_flow

.. doxygenfunction:: nw::graph::maximal_independent_set

.. doxygenfunction:: nw::graph::page_rank

.. doxygenfunction:: nw::graph::prim

.. doxygenfunction:: nw::graph::spMatspMat

.. doxygenfunction:: nw::graph::triangle_count(const GraphT& A)

.. doxygenfunction:: nw::graph::triangle_count_async(std::size_t threads, Op&& op)

.. doxygenfunction:: nw::graph::triangle_count(const Graph& G, std::size_t threads)



Graph Data Structures
---------------------

.. doxygentypedef:: nw::graph::adjacency

.. doxygenclass:: nw::graph::index_adjacency

.. doxygentypedef:: nw::graph::edge_list

.. doxygentypedef:: nw::graph::bi_edge_list

.. doxygenclass:: nw::graph::index_edge_list



Graph Construction
------------------

.. doxygenfunction:: nw::graph::fill_adj_list

.. doxygenfunction:: nw::graph::fill_directed

.. doxygenfunction:: nw::graph::fill_undirected

.. doxygenfunction:: nw::graph::fill(edge_list_t& el, adjacency_t& cs, bool sort_adjacency = false, ExecutionPolicy&& policy = {})

.. doxygenfunction:: nw::graph::fill(edge_list_t& el, adjacency_t& cs, directedness dir, bool sort_adjacency = false, ExecutionPolicy&& policy = {})

.. doxygenfunction:: nw::graph::relabel_by_degree< edge_list_graph edge_list_t, class Vector >

.. doxygenfunction:: nw::graph::relabel_by_degree

.. doxygenfunction:: nw::graph::make_plain_graph

.. doxygenfunction:: nw::graph::make_index_graph

.. doxygenfunction:: nw::graph::make_property_graph

.. doxygenfunction:: nw::graph::join



Range Adaptors
--------------

.. doxygenclass:: nw::graph::back_edge_range

.. doxygenclass:: nw::graph::topdown_bfs_range

.. doxygenclass:: nw::graph::bottomup_bfs_range

.. doxygenclass:: nw::graph::bfs_edge_range

.. doxygenclass:: nw::graph::cyclic_neighbor_range

.. doxygenclass:: nw::graph::cyclic_range_adaptor

.. doxygenclass:: nw::graph::dag_range

.. doxygenclass:: nw::graph::edge_range		  

.. doxygenclass:: nw::graph::filtered_bfs::filtered_bfs_edge_range

.. doxygenclass:: nw::graph::neighbor_range

.. doxygenclass:: nw::graph::plain_range

.. doxygenclass:: nw::graph::reverse_path

.. doxygenclass:: nw::graph::splittable_range_adaptor

.. doxygenclass:: nw::graph::vertex_range

.. doxygenclass:: nw::graph::worklist_range



Graph Generators
----------------



Graph I/O
---------

.. doxygenfunction:: read_mm(const std::string &filename)

.. doxygenfunction:: read_mm(const std::string_& filename2)

.. doxygenfunction:: read_mm(std::istream &inputStream)

.. doxygenfunction:: write_mm(const std::string& filename, edge_list<sym, Attributes...>& A, const std::string& file_symmetry)

.. doxygenfunction:: write_mm(const std::string& filename, adjacency<idx, Attributes...>& A, const std::string& file_symmetry)

.. doxygenfunction:: write_mm(const std::string& filename, biadjacency<idx, Attributes...>& A, const std::string& file_symmetry)



Containers
----------
		  
.. doxygenclass:: nw::graph::array_of_structs

.. doxygencstruct:: nw::graph::struct_of_arrays

.. doxygenclass:: nw::graph::indexed_struct_of_arrays

.. doxygenstruct:: nw::graph::zipped



Utilities
---------

.. doxygenclass:: nw::graph::counting_output_iterator

.. doxygenclass:: nw::graph::par_counting_output_iterator

.. doxygenclass:: nw::util::timer

.. doxygenfunction:: nw::util::proxysort(const ThingToSort& x, std::vector<IntT>& perm, Comparator comp = std::less<IntT>(), ExecutionPolicy policy = {})

.. doxygenfunction:: nw::util::proxysort(const ThingToSort& x, Comparator comp = std::less<IntT>(), ExecutionPolicy policy = {})

.. doxygenfunction:: nw::graph::intersection_size


Experimental Components
-----------------------
