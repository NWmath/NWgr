#include "common/abstract_test.hpp"
#include <concepts>
#include "edge_list.hpp"
#include "compressed.hpp"
#include "graph_concepts.hpp"
#include "vovos.hpp"
#include "aolos.hpp"
/*template<typename GraphT>
requires AdjacencyGraph<GraphT>
void test_func(GraphT& G) {
  
}*/

TEST_CASE("test","[test]") {
    
  adj_list<> A(5);
  A.push_back(0, 1);
  A.push_back(1, 2);
  A.push_back(0, 2);
  A.push_back(1, 3);
  A.push_back(1, 4);
  std::ranges::begin(A);
  for(auto&& out : A) {
    std::cout << "outer" << std::endl;
    for(auto&& in : out) {
      std::cout << "in" << std::endl;
    }
  }
  std::ranges::for_each(A, [](auto&& out){
    std::ranges::for_each(out, [](auto&& in){
      std::cout << "edge to " << std::get<0>(in) << std::endl;});
  });
  //test_func(A);
}
