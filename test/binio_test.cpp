

#include "binio.hpp"
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

  write_bin<directed>("e_foo.el", e);
  auto h = read_bin<directed>("e_foo.el");

  if (e != h) {
    std::cout << "Error h != e" << std::endl;
  } else if (e == h) {
    std::cout << "okay h == e" << std::endl;
  }

  std::cout << "====" << std::endl;

  auto f = read_mm<undirected>(argv[1]);

  f.remove_self_loops();
  f.uniq();

  f.stream_stats(std::cout);
  f.stream_prv(std::cout);

  write_bin<undirected>("f_foo.el", f);
  auto g = read_bin<undirected>("f_foo.el");

  g.stream_stats(std::cout);
  g.stream_prv(std::cout);

  if (f != g) {
    std::cout << "Error g != f" << std::endl;
  } else if (f == g) {
    std::cout << "okay g == f" << std::endl;
  }

  return 0;
}
