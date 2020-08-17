

#include <iostream>
#include "edge_list.hpp"
#include "karate.hpp"

int main() {
  
  {
    edge_list<undirected> a { {0, 1}, {1, 2}} ;
    edge_list<directed>   b { {0, 2}, {1, 0}, {0, 1}} ;
    
    edge_list<directed>   c = a.convert_directedness<directed>();
    edge_list<undirected> d = b.convert_directedness<undirected>();
    edge_list<directed>   e = b.convert_directedness<directed>();
    edge_list<undirected> f = a.convert_directedness<undirected>();
    
    a.stream_edges(std::cout); std::cout << std::endl;
    b.stream_edges(std::cout); std::cout << std::endl;
    c.stream_edges(std::cout); std::cout << std::endl;
    d.stream_edges(std::cout); std::cout << std::endl;
    e.stream_edges(std::cout); std::cout << std::endl;
    f.stream_edges(std::cout); std::cout << std::endl;
  }

  {
    edge_list<undirected, double> a { {0, 1, 3.1}, {1, 2, 4.159}} ;
    edge_list<directed, double>   b { {0, 2, 42.2}, {1, 0, 24.4}, {0, 1, 81.9}} ;
    
    auto c = a.convert_directedness<directed>();
    auto d = b.convert_directedness<undirected>();
    auto e = b.convert_directedness<directed>();
    auto f = a.convert_directedness<undirected>();
    
    a.stream_edges(std::cout); std::cout << std::endl;
    b.stream_edges(std::cout); std::cout << std::endl;
    c.stream_edges(std::cout); std::cout << std::endl;
    d.stream_edges(std::cout); std::cout << std::endl;
    e.stream_edges(std::cout); std::cout << std::endl;
    f.stream_edges(std::cout); std::cout << std::endl;
  }


  return 0;
}
