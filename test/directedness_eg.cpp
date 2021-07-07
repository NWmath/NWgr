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

#include "nwgraph/edge_list.hpp"
#include "karate.hpp"

using namespace nw::graph;
using namespace nw::util;

int main() {

  {
    edge_list<directedness::undirected> a{{0, 1}, {1, 2}};
    edge_list<directedness::directed>   b{{0, 2}, {1, 0}, {0, 1}};

    edge_list<directedness::directed>   c = a.convert_directedness<directedness::directed>();
    edge_list<directedness::undirected> d = b.convert_directedness<directedness::undirected>();
    edge_list<directedness::directed>   e = b.convert_directedness<directedness::directed>();
    edge_list<directedness::undirected> f = a.convert_directedness<directedness::undirected>();

    a.stream_edges(std::cout);
    std::cout << std::endl;
    b.stream_edges(std::cout);
    std::cout << std::endl;
    c.stream_edges(std::cout);
    std::cout << std::endl;
    d.stream_edges(std::cout);
    std::cout << std::endl;
    e.stream_edges(std::cout);
    std::cout << std::endl;
    f.stream_edges(std::cout);
    std::cout << std::endl;
  }

  {
    edge_list<directedness::undirected, double> a{{0, 1, 3.1}, {1, 2, 4.159}};
    edge_list<directedness::directed, double>   b{{0, 2, 42.2}, {1, 0, 24.4}, {0, 1, 81.9}};

    auto c = a.convert_directedness<directedness::directed>();
    auto d = b.convert_directedness<directedness::undirected>();
    auto e = b.convert_directedness<directedness::directed>();
    auto f = a.convert_directedness<directedness::undirected>();

    a.stream_edges(std::cout);
    std::cout << std::endl;
    b.stream_edges(std::cout);
    std::cout << std::endl;
    c.stream_edges(std::cout);
    std::cout << std::endl;
    d.stream_edges(std::cout);
    std::cout << std::endl;
    e.stream_edges(std::cout);
    std::cout << std::endl;
    f.stream_edges(std::cout);
    std::cout << std::endl;
  }

  return 0;
}
