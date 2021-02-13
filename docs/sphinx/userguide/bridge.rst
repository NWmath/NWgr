


From Graph Theory to Programming with Graphs
============================================




Although the abstract / theoretical terminology for graphs is that a
graph :math:`G = (V,\ E)`\ and so on, in fact, graph algorithms are
based on efficient concrete representations of the relationships induced
on :math:`V` by :math:`\text{E.}` In particular, graph algorithms
require efficient access to this structure. Graph algorithms are
interesting because of the structure and so almost all graph algorithms
require efficient traversal, and hence require an index adjacency list
to operate over. (A few graph algorithms only require access to the
edges without the need to traverse the structure and hence only need the
index edge list rather than the adjacency list.) Out of 30 algorithms
examined from Boost.Graph, only one did not require an index adjacency
list. Unfortunately, it is often the case that :math:`G`\ and
:math:`\text{Adj}` will both be referred to as “the graph,” and, in
fact, it is most often the case that when someone refers to “the graph,”
they actually mean :math:`\text{Adj.}` It is nonetheless important to
maintain the distinction between the graph :math:`G`\ and :math:`Adj,`
the index adjacency list over it.

As with the STL, our goal with this graph library proposal is to present
a systematic organization of the domain of graph algorithms, consisting
of a set of minimal type requirements for graph algorithms (aka
“concepts” in the general sense), an API for realizing those type
requirements in C++, a set of algorithms, and some representative data
structures that might not be trivially realizable using composed
containers from the standard library.

The library follows the principles of generic programming. That is, it
is algorithm-centric and algorithms are realized as function templates
with well-defined requirements. These type requirements are organized
into a small set of related abstractions such that the algorithms
defined with them can operate correctly and efficiently on the widest
possible set of input types. In particular, it is essential that the
library fully support composition with and among third-party libraries.

Consequently, the library is not built around a single graph type or
even a single graph abstraction. We will group requirements together and
use graph terminology to name those sets of requirements.





Basic Concepts
~~~~~~~~~~~~~~

Based on our observations of graphs and their representations, an index
adjacency structure of a graph (i.e., :math:`ADj(G)`) is a range of
ranges, specifically, a random access range of forward ranges. As such,
the outer range conforms completely to the requirements of the
random_access_range concept and the inner range conforms completely to
the requirements of the forward_range concept, including all valid
expressions and associated types (such as begin, end, etc.).

The size of a graph is the cardinality of its vertex set. This can be
accessed via the size() function on the outer range or via the
num_vertices() function on the outer range.

The vertices and edges of a graph are abstract notions that are only implicitly
reified in a program via vertex and edge properties (see examples above). However,
these properties do need handles in order to be accessed, and the index adjacency
structure of the graph needs indices in order to be traversed. Accordingly, we have
**vertex keys** and **edge keys**.


The outer range of a graph is indexed with a vertex_key. Indexing into
the outer range with a vertex key :math:`u` returns an inner range,
corresponding to the set of edges or vertices reachable from
:math:`u,`\ i.e., it contains information about all :math:`(u,v)`\ in
the edge set of the graph. The objects stored by the inner range
associated with :math:`u` can be accessed to obtain the source vertex
key :math:`u,`\ the target vertex key :math:`v,` and (optionally) an
edge key corresponding to the edge :math:`(u,v),` or the properties
associated with the edge :math:`(u,v).` The former case is more flexible
and allows the actual edge properties to be accessed, but requires an
indirection. The latter case may involve copying property values, but
avoids the extra indirection. We refer to this type of graph as an
**incidence graph**. The neighborhood range associated with a given
vertex is accessed using the function **out_edges**.

The following code assumes the inner range stores pairs of vertices,
corresponding to the edges leaving each vertex), and would iterate
through all of the edges in the graph:

|image8|

Note that a graph composed of standard library containers, e.g.,
std:vector<std::forward_list<std::tuple<int, int>>> could be used in the
example above.

For a select few algorithms, we may also need the information about
every :math:`(v,u)`, given :math:`\text{u.}` (This is essentially the
transpose of :math:`(u,v)`). A graph that stores information for both
:math:`(v,u)` and :math:`(u,v)` is called a **bidirectional graph**
(which is necessarily also an incidence graph). The “transpose”
neighborhood range associated with a given vertex is accessed using the
function **in_edges**.

Some graph algorithms require knowing the degree of each vertex (the
size of each neighborhood). In that case, the inner range should also be
a sized range. The degree of each vertex can be accessed with the range
function size on the inner range or with the degree() function (which
takes a vertex key as argument). The degree must be accessible in
constant time.

In rare cases a graph algorithm will require knowing the total number of
edges in a graph. If all of the edges are stored in a single
sized_range, this can be obtained from the size function on that range.
In other cases, the number of edges will need to be maintained as part
of the graph. The total number of edges of a graph can be obtained with
the num_edges() function (which takes a graph as argument).

Graph Traversal
~~~~~~~~~~~~~~~

Traversing a graph is accomplished by iterating over the ranges
comprising the graph. The iterator types are those associated with each
range type. We refer to the outer range of the graph as a **vertex
range** and its corresponding iterator as a **vertex iterator**. An
**out edge iterator** is used to iterate over the out edges associated
with a vertex in an incidence graph and an **in edge iterator** is used
to iterate over the in edges of a bidirectional graph.

An example traversing an incidence graph is shown below.




Vertex and Edge Descriptors
---------------------------


Vertex and Edge Properties
--------------------------
