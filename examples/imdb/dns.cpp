#include <fstream>
#include <iostream>

#include <map>
#include <string>
#include <vector>
#include <queue>
#include <deque>

#include "xtensor/xcsv.hpp"

#include "mmio.hpp"
#include "compressed.hpp"
#include "edge_list.hpp"
#include "util/timer.hpp"


int main(int argc, char*argv[]) {

  nw::graph::edge_list<nw::graph::directed> edges = nw::graph::read_mm<nw::graph::directed>(argv[1]);

  nw::util::timer t4("build biadjacencies");
  
  auto H = nw::graph::adjacency<0>(edges);
  auto G = nw::graph::adjacency<1>(edges);

  t4.stop(); 
  std::cout << t4 << std::endl;

  nw::util::timer t5("build s_overlap");

  nw::graph::edge_list<nw::graph::undirected> s_overlap;
  s_overlap.open_for_push_back();
  
  size_t s = 2;

#pragma omp parallel for schedule(dynamic, 64)

  for (size_t i = 0; i < H.size(); ++i) {

#if 0  // progress meter of sorts
    if ((i % 8192) == 0) {
      std::cout << i << std::endl;
    }
#endif

    std::map<size_t, size_t> K;

    for (auto && [k] : H[i]) {
      for (auto && [j] : G[k]) {
	if (j > i) {
	  K[j]++;
	}
      }      
    }

#pragma omp critical
    for (auto && [key, val] : K) {
      if (val >= s) {
	s_overlap.push_back(i, key);
      }
    }

  }
  s_overlap.close_for_push_back();

  t5.stop(); 
  std::cout << t5 << std::endl;

  nw::util::timer t6("build s_overlap adjacency");

  auto L = nw::graph::adjacency<0>(s_overlap);

  t6.stop(); 
  std::cout << t6 << std::endl;

  return 0;
}
