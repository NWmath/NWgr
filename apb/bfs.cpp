

#include <iostream>
#include <vector>

#include "compressed.hpp"
#include "edge_list.hpp"
#include "mmio.hpp"
#include "edge_range.hpp"

#if defined(CL_SYCL_LANGUAGE_VERSION)
#include <dpstd/iterator>
#else
#include "tbb/iterators.h"
#endif

using namespace nw::graph;
using namespace nw::util;

template<typename Adjacency>
auto apb_adj(Adjacency& graph, vertex_id_t seed) {

  vertex_id_t        N = graph.max() + 1;
  std::vector<vertex_id_t> p(N);

  std::cout << "edge_range\n";
  std::fill(p.begin(), p.end(), std::numeric_limits<vertex_id_t>::max());
  {
    life_timer _("raw for loop");

    auto ptr = graph.indices_.data();
    auto idx = std::get<0>(graph.to_be_indexed_).data();
    
    std::queue<vertex_id_t> Q;
    Q.push(seed);
    while(!Q.empty()) {
      vertex_id_t vtx = Q.front();
      Q.pop();
      for (auto n = ptr[vtx]; n < ptr[vtx + 1]; ++n) {
	if(p[idx[n]] == std::numeric_limits<vertex_id_t>::max()) {
	  p[idx[n]] = vtx;
	  Q.push(idx[n]);
	}
      }
    }
  }

  std::fill(p.begin(), p.end(), std::numeric_limits<vertex_id_t>::max());
  {
    life_timer _("iterator based for loop");

    std::queue<vertex_id_t> Q;
    Q.push(seed);
    while(!Q.empty()) {
      vertex_id_t vtx = Q.front();
      Q.pop();
      for (auto n = graph[vtx].begin(); n != graph[vtx].end(); ++n) {
	if(p[std::get<0>(*n)] == std::numeric_limits<vertex_id_t>::max()) {
	  p[std::get<0>(*n)] = vtx;
	  Q.push(std::get<0>(*n));
	}
      }
    }
  }


  std::fill(p.begin(), p.end(), std::numeric_limits<vertex_id_t>::max());
  {
    life_timer _("range based for loop");
    
    std::queue<vertex_id_t> Q;
    Q.push(seed);
    while(!Q.empty()) {
      vertex_id_t vtx = Q.front();
      Q.pop();
      for (auto n : graph[vtx]) {
	if(p[std::get<0>(n)] == std::numeric_limits<vertex_id_t>::max()) {
	  p[std::get<0>(n)] = vtx;
	  Q.push(std::get<0>(n));
	}
      }
    }  
  }
    
  std::fill(p.begin(), p.end(), std::numeric_limits<vertex_id_t>::max());
  {
    life_timer _("range based for loop with compound initializer");

    std::queue<vertex_id_t> Q;
    Q.push(seed);
    while(!Q.empty()) {
      vertex_id_t vtx = Q.front();
      Q.pop();
      for (auto&& [n] : graph[vtx]) {
	if(p[n] == std::numeric_limits<vertex_id_t>::max()) {
	  p[n] = vtx;
	  Q.push(n);
	}
      }
    }  
  }

  std::fill(p.begin(), p.end(), std::numeric_limits<vertex_id_t>::max());
  {
    life_timer _("std::for_each");
    
    std::queue<vertex_id_t> Q;
    Q.push(seed);
    while(!Q.empty()) {
      vertex_id_t vtx = Q.front();
      Q.pop();
      std::for_each(graph[vtx].begin(), graph[vtx].end(), [&](auto&& n) {
        if(p[std::get<0>(n)] == std::numeric_limits<vertex_id_t>::max()) {
	  p[std::get<0>(n)] = vtx;
	  Q.push(std::get<0>(n));
	}
      });
    }
  }
}

void usage(const std::string& msg = "") { std::cout << std::string("Usage: ") + msg + " " << std::endl; }

int main(int argc, char* argv[]) {
  std::string edgelistFile             = "";
  std::string versions                 = "0";
  std::string read_processed_edgelist  = "";
  std::string write_processed_edgelist = "";

  bool         verbose      = false;
  bool         debug        = false;
  size_t       nthread      = 1; (void)nthread; // silence warnings
  size_t       ntrial       = 1; (void)ntrial;  // silence warnings
  const size_t max_versions = 16;

  for (int argIndex = 1; argIndex < argc; ++argIndex) {
    std::string arg(argv[argIndex]);

    if (arg == "--edgelistfile" || arg == "-f" || arg == "-i") {
      if (++argIndex == argc) {
        usage(argv[0]);
      }
      edgelistFile = std::string(argv[argIndex]);
    } else if (arg == "--read_processed_edgelist") {
      if (++argIndex == argc) {
        usage(argv[0]);
      }
      read_processed_edgelist = std::string(argv[argIndex]);
    } else if (arg == "--write_processed_edgelist") {
      if (++argIndex == argc) {
        usage(argv[0]);
      }
      write_processed_edgelist = std::string(argv[argIndex]);
    } else if (arg == "--ntrial" || arg == "--ntrials") {
      if (++argIndex == argc) {
        usage(argv[0]);
      }
      ntrial = std::stoi(argv[argIndex]);
    } else if (arg == "--nthread" || arg == "--nthreads") {
      if (++argIndex == argc) {
        usage(argv[0]);
      }
      nthread = std::stoi(argv[argIndex]);
    } else if (arg == "--version" || arg == "--versions") {
      if (++argIndex == argc) {
        usage(argv[0]);
      }
      versions = std::string(argv[argIndex]);
    } else if (arg == "-d") {
      debug = true;
    } else if (arg == "-v") {
      verbose = true;
    } else {
      usage(argv[0]);
      return -1;
    }
  }

  auto el_a = [&]() {
    if (read_processed_edgelist != "") {
      life_timer                  _("deserialize");
      edge_list<directed> el_a(0);
      el_a.deserialize(read_processed_edgelist);
      return el_a;
    } else if (edgelistFile != "") {
      life_timer _("read mm");
      return read_mm<directed>(edgelistFile);
    } else {
      usage(argv[0]);
      return edge_list<directed>(0);
    }
  }();

  if (verbose) {
    el_a.stream_stats();
  }

  if (write_processed_edgelist != "") {
    el_a.serialize(write_processed_edgelist);
  }

  //  apb_el(el_a);

  auto adj_a = [&]() {
    life_timer _("build");
    return adjacency<1>(el_a);
  }();

  if (verbose) {
    adj_a.stream_stats();
  }
  if (debug) {
    adj_a.stream_indices();
  }

  apb_adj(adj_a, 0);

  return 0;
}
