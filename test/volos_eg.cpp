

#include <forward_list>
#include <tuple>
#include <vector>

#include "nwgraph/volos.hpp"

#include "nwgraph/graph_concepts.hpp"

int main() {

  using T = std::vector<std::forward_list<std::tuple<int, int, double>>>;

  static_assert(nw::graph::graph<T>);
  static_assert(nw::graph::vertex_list_c<std::forward_list<std::tuple<int, int, double>>>);
  static_assert(nw::graph::adjacency_graph<std::vector<std::forward_list<std::tuple<int, int, double>>>>);

  return 0;
}
