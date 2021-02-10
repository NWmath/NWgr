
// #include "graph_traits.hpp"

#include <forward_list>
#include <tuple>
#include <vector>

#include "containers/adjacency.hpp"
#include "containers/edge_list.hpp"
#include "containers/volos.hpp"
#include "containers/vovos.hpp"

namespace nw {
namespace graph {

template <typename>
struct is_tuple : std::false_type {};
template <typename... T>
struct is_tuple<std::tuple<T...>> : std::true_type {};

template <typename G, typename T = G>
requires std::ranges::random_access_range<T>&& std::ranges::forward_range<typename T::value_type>&&
                                               is_tuple<typename T::value_type::value_type>::value struct graph_traits_x {
private:
  using tuple_type       = typename T::value_type::value_type;
  using vertex_size_type = typename T::size_type;

public:
  using vertex_id_type    = typename std::tuple_element<0, tuple_type>::type;
  using num_vertices_type = std::array<vertex_size_type, 1>;
};

using ii = graph_traits_x<std::vector<std::forward_list<std::tuple<int>>>>::vertex_id_type;
using jj = graph_traits_x<nw::graph::adjacency<0>>::vertex_id_type;

}    // namespace graph
}    // namespace nw


int main() { return 0; }
