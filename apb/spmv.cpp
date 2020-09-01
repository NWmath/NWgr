

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
auto apb_adj(Adjacency& graph) {

  vertex_id_t        N = graph.max() + 1;
  std::vector<float> x(N), y(N);
  std::iota(x.begin(), x.end(), 0);

  {
    auto per = make_edge_range<0>(graph);

    std::cout << "edge_range\n";

    std::fill(y.begin(), y.end(), 0);
    {
      life_timer _("raw for loop");

      auto ptr = graph.indices_.data();
      auto idx = std::get<0>(graph.to_be_indexed_).data();
      auto dat = std::get<1>(graph.to_be_indexed_).data();

      for (vertex_id_t i = 0; i < N; ++i) {
        for (auto j = ptr[i]; j < ptr[i + 1]; ++j) {
          y[i] += x[idx[j]] * dat[j];
        }
      }
    }

    std::fill(y.begin(), y.end(), 0);
    {
      life_timer _("iterator based for loop with iterator based for loop");

      vertex_id_t k = 0;
      for (auto i = graph.begin(); i != graph.end(); ++i) {
        for (auto j = (*i).begin(); j != (*i).end(); ++j) {
          y[k] += x[std::get<0>(*j)] * std::get<1>(*j);
        }
        ++k;
      }
    }

     std::fill(y.begin(), y.end(), 0);
    {
      life_timer _("range based for loop with range based for loop with compound initializer");

      vertex_id_t k = 0;
      for (auto&& i : graph) {
        for (auto&& [j, v] : i) {
          y[k] += x[j] * v;
        }
        ++k;
      }
    }

    std::fill(y.begin(), y.end(), 0);
    {
      life_timer _("range based for loop auto");

      for (auto j : per) {
        y[std::get<0>(j)] += x[std::get<1>(j)] * std::get<2>(j);
      }
    }

    std::fill(y.begin(), y.end(), 0);
    {
      life_timer _("range based for loop auto &&");

      for (auto&& j : per) {
        y[std::get<0>(j)] += x[std::get<1>(j)] * std::get<2>(j);
      }
    }

    std::fill(y.begin(), y.end(), 0);
    {
      life_timer _("range based for loop compound initialization auto");

      for (auto [i, j, v] : per) {
        y[i] += x[j] * v;
      }
    }

    std::fill(y.begin(), y.end(), 0);
    {
      life_timer _("range based for loop compound initialization auto &&");

      for (auto&& [i, j, v] : per) {
        y[i] += x[j] * v;
      }
    }

    std::fill(y.begin(), y.end(), 0);
    {
      life_timer _("std for_each auto");

      std::for_each(per.begin(), per.end(), [&](auto j) { y[std::get<0>(j)] += x[std::get<1>(j)] * std::get<2>(j); });
    }

    std::fill(y.begin(), y.end(), 0);
    {
      life_timer _("std for_each auto &&");

      std::for_each(per.begin(), per.end(), [&](auto&& j) { y[std::get<0>(j)] += x[std::get<1>(j)] * std::get<2>(j); });
    }
  }
  if constexpr(false){
    auto per = edge_range(graph);

    std::cout << "edge_range" << std::endl;

    std::fill(y.begin(), y.end(), 0);
    {
      life_timer _("raw for loop");

      auto ptr = graph.indices_.data();
      auto idx = std::get<0>(graph.to_be_indexed_).data();
      auto dat = std::get<1>(graph.to_be_indexed_).data();

      for (vertex_id_t i = 0; i < N; ++i) {
        for (auto j = ptr[i]; j < ptr[i + 1]; ++j) {
          y[i] += x[idx[j]] * dat[j];
        }
      }
    }

    std::fill(y.begin(), y.end(), 0);
    {
      life_timer _("iterator based for loop with iterator based for loop");

      vertex_id_t k = 0;
      for (auto i = graph.begin(); i != graph.end(); ++i) {
        for (auto j = (*i).begin(); j != (*i).end(); ++j) {
          y[k] += x[std::get<0>(*j)] * std::get<1>(*j);
        }
        ++k;
      }
    }



    std::fill(y.begin(), y.end(), 0);
    {
      life_timer _("range based for loop auto");

      for (auto j : per) {
        y[std::get<0>(j)] += x[std::get<1>(j)] * std::get<2>(j);
      }
    }

    std::fill(y.begin(), y.end(), 0);
    {
      life_timer _("range based for loop auto &&");

      for (auto&& j : per) {
        y[std::get<0>(j)] += x[std::get<1>(j)] * std::get<2>(j);
      }
    }

    std::fill(y.begin(), y.end(), 0);
    {
      life_timer _("range based for loop compound initialization auto");

      for (auto [i, j, v] : per) {
        y[i] += x[j] * v;
      }
    }

    std::fill(y.begin(), y.end(), 0);
    {
      life_timer _("range based for loop compound initialization auto &&");

      for (auto&& [i, j, v] : per) {
        y[i] += x[j] * v;
      }
    }

    std::fill(y.begin(), y.end(), 0);
    {
      life_timer _("std for_each auto");

      std::for_each(per.begin(), per.end(), [&](auto j) { y[std::get<0>(j)] += x[std::get<1>(j)] * std::get<2>(j); });
    }

    std::fill(y.begin(), y.end(), 0);
    {
      life_timer _("std for_each auto &&");

      std::for_each(per.begin(), per.end(), [&](auto&& j) { y[std::get<0>(j)] += x[std::get<1>(j)] * std::get<2>(j); });
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
      edge_list<directed, double> el_a(0);
      el_a.deserialize(read_processed_edgelist);
      return el_a;
    } else if (edgelistFile != "") {
      life_timer _("read mm");
      return read_mm<directed, double>(edgelistFile);
    } else {
      usage(argv[0]);
      return edge_list<directed, double>(0);
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
    return adjacency<1, double>(el_a);
  }();

  if (verbose) {
    adj_a.stream_stats();
  }
  if (debug) {
    adj_a.stream_indices();
  }

  apb_adj(adj_a);

  return 0;
}
