//
// This file is part of BGL17 (aka NWGraph aka GraphPack aka the Graph Standard Library)
// (c) Pacific Northwest National Laboratory 2018
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#include <iostream>

#include "containers/edge_list.hpp"
#include "karate.hpp"

using namespace nw::graph;
using namespace nw::util;

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