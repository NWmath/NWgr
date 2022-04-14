.. SPDX-FileCopyrightText: 2022 Batelle Memorial Institute
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

.. doxygenfunction:: nw::graph::betweenness_brandes

.. doxygenfunction:: nw::graph::bfs_v0

.. doxygenfunction:: nw::graph::bfs_v11

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

.. doxygenfunction:: nw::graph::mis_algorithm

.. doxygenfunction:: nw::graph::page_rank_v11

.. doxygenfunction:: nw::graph::prim

.. doxygenfunction:: nw::graph::spMatspMat

.. doxygenfunction:: nw::graph::triangle_count_v0

.. doxygenfunction:: nw::graph::triangle_count_async



Graph Data Structures
---------------------

.. doxygentypedef:: nw::graph::adjacency

.. doxygenclass:: nw::graph::index_adjacency

.. doxygentypedef:: nw::graph::edge_list

.. doxygentypedef:: nw::graph::bi_edge_list

.. doxygenclass:: nw::graph::index_edge_list



Graph Construction
------------------



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
