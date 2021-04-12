

#include <forward_list>
#include <tuple>
#include <vector>

#include "nwgraph/volos.hpp"

#include "nwgraph/graph_concepts.hpp"

int main() {

  using T = nw::graph::adj_list<0>;
  static_assert(nw::graph::graph<T>);
  static_assert(nw::graph::adjacency_graph<T>);

  return 0;
}
