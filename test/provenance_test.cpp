

#include "edge_list.hpp"
#include "io/mmio.hpp"
#include <cxxabi.h>
#include <iostream>
#include <typeinfo>

using namespace nw::graph;
using namespace nw::util;

void usage(const std::string& msg) { std::cout << msg << std::endl; }

int main(int argc, char* argv[]) {

  if (argc != 2) {
    usage(argv[0]);
    return -1;
  }

  auto e = read_mm<directed>(argv[1]);

  e.remove_self_loops();
  e.uniq();

  e.stream_stats(std::cout);
  e.stream_prv(std::cout);

  auto f = read_mm<undirected>(argv[1]);

  f.remove_self_loops();
  f.uniq();

  f.stream_stats(std::cout);
  f.stream_prv(std::cout);

  return 0;
}
