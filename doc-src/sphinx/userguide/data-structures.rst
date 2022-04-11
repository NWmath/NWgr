
.. _`sec:data_structures`:

Model Data Structures
=====================

In (lifting) , we have demonstrated that the built-in types in the
standard library are sufficient to construct a graph. Additionally, we
discussed that any data structure meeting the requirements specified by
the NWGraph concepts can be composed with the NWGraph algorithms based
on those concepts. For instance,
``std::vector<std::vector< std::tuple<size_t, double>>>`` meets the
requirements of ``adjacency_list`` concept, hence can be used with any
of the appropriate algorithms. Beyond simple composition of standard
library components are data structures such as compressed sparse
structures (we note that as far as a graph is concerned, compressed
sparse row (CSR) is structurally same as compressed sparse column),
which offer higher performance. Even though a compressed structure is
not technically made as a range of ranges, it can provide the interface
of a range of ranges. The workhorse graph structure for NWGraph is
``nwgraph::adjacency``, with the following (highly abbreviated)
interface:

.. code:: cpp

   template <..>
     class adjacency {
       class outer_iterator {
         using iterator_category = 
         std::random_access_iterator_tag; ..};
       class inner_iterator;
       outer_iterator begin();
       outer_iterator end();
       operator[](index_t i) const;  };

Here, to make CSR equivalent as an ``adjacency_list`` (range of ranges),
we define a private iterator type that acts as a random access iterator.

NWGraph has a small set of utility functions for building graphs from a
given dataset in a generic fashion. The first step involves building an
index edge list, given a vertex table and edge table. The second step is
to build an index graph (that is, filling in a structure modeling
adjacency), given an index edge list. Oftentimes, it is needed to have
sorted data in the neighborhood range. The graph construction algorithms
take a runtime flag that indicate whether sorting should be done during
graph construction. We also have functions to sort graphs that have
already been constructed. The pertinent APIs for graph construction are
shown below:

.. code:: cpp

   template <class IndexEdgeList, class VRange, class ERange>
   IndexEdgeList make_index_edge_list(const VRange& vertices, const ERange& edges);

   template <adjacency_list Graph, class IndexEdgeList>
   Graph make_graph(const IndexEdgeList& edge_list);

.. container::
   :name: table:graphs_dataset

   .. table:: Dataset for performance evaluation

      +---------+----------+----------+----------+----------+----------+
      | Name    | Des      | #        | #Edges   | Degree   | Re       |
      |         | cription | Vertices | (M)      | Dist     | ferences |
      |         |          | (M)      |          | ribution |          |
      +=========+==========+==========+==========+==========+==========+
      | Road    | USA road | 23.9     | 57.7     | bounded  | :r       |
      |         | network  |          |          |          | aw-latex |
      |         |          |          |          |          | :`\cite` |
      +---------+----------+----------+----------+----------+----------+
      | Twitter | Twitter  | 61.6     | 1,468.4  | power    | :r       |
      |         | follower |          |          |          | aw-latex |
      |         | Links    |          |          |          | :`\cite` |
      +---------+----------+----------+----------+----------+----------+
      | Web     | Web      | 50.6     | 1,930.3  | power    | :r       |
      |         | Crawl of |          |          |          | aw-latex |
      |         | .sk      |          |          |          | :`\cite` |
      |         | Domain   |          |          |          |          |
      +---------+----------+----------+----------+----------+----------+
      | Kron    | S        | 134.2    | 2,111.6  | power    | :r       |
      |         | ynthetic |          |          |          | aw-latex |
      |         | Graph    |          |          |          | :`\cite` |
      +---------+----------+----------+----------+----------+----------+
      | Urand   | Uniform  | 134.2    | 2,147.5  | normal   | :r       |
      |         | Random   |          |          |          | aw-latex |
      |         | Graph    |          |          |          | :`\cite` |
      +---------+----------+----------+----------+----------+----------+
