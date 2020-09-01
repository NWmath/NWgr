

#include <iostream>
#include <vector>

#include "compressed.hpp"
#include "edge_list.hpp"
#include "mmio.hpp"
#include "edge_range.hpp"
#include "plain_range.hpp"

#if defined(EXECUTION_POLICY)
#if defined(CL_SYCL_LANGUAGE_VERSION)
#include <dpstd/iterators.h>
namespace nw::graph {
template <class T>
using counting_iterator = dpstd::counting_iterator<T>;
}
#else
#include <tbb/iterators.h>
namespace nw::graph {
template <class T>
using counting_iterator = tbb::counting_iterator<T>;
}
#endif
#endif


using namespace nw::graph;
using namespace nw::util;

template<typename Adjacency>
auto apb_adj(Adjacency& graph) {

  vertex_id_t              N = graph.max() + 1;
  std::vector<vertex_id_t> degrees(N);

  {
    auto pr = plain_range(graph);

    std::cout << "plain_range" << std::endl;

    degrees.clear();
    degrees.resize(N);
    {
      life_timer _("iterator based for loop");

      for (auto j = pr.begin(); j != pr.end(); ++j) {
        ++degrees[std::get<0>(*j)];
      }
    }

    degrees.clear();
    degrees.resize(N);
    {
      life_timer _("range based for loop auto");

      for (auto j : pr) {
        ++degrees[std::get<0>(j)];
      }
    }

    degrees.clear();
    degrees.resize(N);
    {
      life_timer _("range based for loop auto &&");

      for (auto&& j : pr) {
        ++degrees[std::get<0>(j)];
      }
    }

    degrees.clear();
    degrees.resize(N);
    {
      life_timer _("range based for loop compound initialization auto");

      for (auto [j] : pr) {
        ++degrees[j];
      }
    }

    degrees.clear();
    degrees.resize(N);
    {
      life_timer _("range based for loop compound initialization auto &&");

      for (auto&& [j] : pr) {
        ++degrees[j];
      }
    }

    degrees.clear();
    degrees.resize(N);
    {
      life_timer _("indexed for loop");

      for (vertex_id_t j = 0; j < N; ++j) {
        ++degrees[j];
      }
    }

    degrees.clear();
    degrees.resize(N);
    {
      life_timer _("std for_each auto");

      std::for_each(pr.begin(), pr.end(), [&](auto j) { ++degrees[std::get<0>(j)]; });
    }

    degrees.clear();
    degrees.resize(N);
    {
      life_timer _("std for_each auto &&");

      std::for_each(pr.begin(), pr.end(), [&](auto&& j) { ++degrees[std::get<0>(j)]; });
    }

    degrees.clear();
    degrees.resize(N);
    {
      life_timer _("counting iterator");

      std::for_each(counting_iterator<vertex_id_t>(0), counting_iterator<vertex_id_t>(N), [&](auto j) { ++degrees[j]; });
    }
  }
  {
    auto per = edge_range(graph);

    std::cout << "edge_range 0" << std::endl;

    degrees.clear();
    degrees.resize(N);
    {
      life_timer _("raw for loop");

      auto ptr = graph.indices_.data();
      auto idx = std::get<0>(graph.to_be_indexed_).data();

      for (vertex_id_t i = 0; i < N; ++i) {
    for (auto j = ptr[i]; j < ptr[i+1]; ++j) {
      ++degrees[i];
    }
      }
    }


    degrees.clear();
    degrees.resize(N);
    {
      life_timer _("iterator based for loop");

      for (auto j = per.begin(); j != per.end(); ++j) {
        ++degrees[std::get<0>(*j)];
      }
    }

    degrees.clear();
    degrees.resize(N);
    {
      life_timer _("range based for loop auto");

      for (auto j : per) {
        ++degrees[std::get<0>(j)];
      }
    }

    degrees.clear();
    degrees.resize(N);
    {
      life_timer _("range based for loop auto &&");

      for (auto&& j : per) {
        ++degrees[std::get<0>(j)];
      }
    }

    degrees.clear();
    degrees.resize(N);
    {
      life_timer _("range based for loop compound initialization auto");

      for (auto [j, i] : per) {
        ++degrees[j];
      }
    }

    degrees.clear();
    degrees.resize(N);
    {
      life_timer _("range based for loop compound initialization auto &&");

      for (auto&& [i, j] : per) {
        ++degrees[i];
      }
    }

    degrees.clear();
    degrees.resize(N);
    {
      life_timer _("indexed for loop");

      auto gr = graph.begin();
      for (vertex_id_t i = 0; i < N; ++i) {
        for (auto j = gr[i].begin(); j != gr[i].end(); ++j) {
          ++degrees[i];
        }
      }
    }

    degrees.clear();
    degrees.resize(N);
    {
      life_timer _("std for_each auto");

      std::for_each(per.begin(), per.end(), [&](auto j) { ++degrees[std::get<0>(j)]; });
    }

    degrees.clear();
    degrees.resize(N);
    {
      life_timer _("std for_each auto &&");

      std::for_each(per.begin(), per.end(), [&](auto&& j) { ++degrees[std::get<0>(j)]; });
    }

    degrees.clear();
    degrees.resize(N);
    {
      life_timer _("counting iterator");

      auto gr = graph.begin();
      std::for_each(counting_iterator<vertex_id_t>(0), counting_iterator<vertex_id_t>(N), [&](auto i) {
        for (auto j = gr[i].begin(); j != gr[i].end(); ++j) {
          ++degrees[i];
        }
      });
    }
  }

  {

    auto per = edge_range(graph);

    std::cout << "edge_range 1" << std::endl;

    degrees.clear();
    degrees.resize(N);
    {
      life_timer _("raw for loop");

      auto ptr = graph.indices_.data();
      auto idx = std::get<0>(graph.to_be_indexed_).data();

      for (vertex_id_t i = 0; i < N; ++i) {
    for (auto j = ptr[i]; j < ptr[i+1]; ++j) {
      ++degrees[idx[j]];
    }
      }
    }

    degrees.clear();
    degrees.resize(N);
    {
      life_timer _("iterator based for loop");

      for (auto j = per.begin(); j != per.end(); ++j) {
        ++degrees[std::get<1>(*j)];
      }
    }

    degrees.clear();
    degrees.resize(N);
    {
      life_timer _("range based for loop auto");

      for (auto j : per) {
        ++degrees[std::get<1>(j)];
      }
    }

    degrees.clear();
    degrees.resize(N);
    {
      life_timer _("range based for loop auto &&");

      for (auto&& j : per) {
        ++degrees[std::get<1>(j)];
      }
    }

    degrees.clear();
    degrees.resize(N);
    {
      life_timer _("range based for loop compound initialization auto");

      for (auto [i, j] : per) {
        ++degrees[j];
      }
    }

    degrees.clear();
    degrees.resize(N);
    {
      life_timer _("range based for loop compound initialization auto &&");

      for (auto&& [i, j] : per) {
        ++degrees[j];
      }
    }

    degrees.clear();
    degrees.resize(N);
    {
      life_timer _("indexed for loop");

      auto gr = graph.begin();
      for (vertex_id_t i = 0; i < N; ++i) {
        for (auto j = gr[i].begin(); j != gr[i].end(); ++j) {
          ++degrees[std::get<0>(*j)];
        }
      }
    }

    degrees.clear();
    degrees.resize(N);
    {
      life_timer _("std for_each auto");

      std::for_each(per.begin(), per.end(), [&](auto j) { ++degrees[std::get<1>(j)]; });
    }

    degrees.clear();
    degrees.resize(N);
    {
      life_timer _("std for_each auto &&");

      std::for_each(per.begin(), per.end(), [&](auto&& j) { ++degrees[std::get<1>(j)]; });
    }

    degrees.clear();
    degrees.resize(N);
    {
      life_timer _("counting iterator with iterator for");

      auto gr = graph.begin();
      std::for_each(counting_iterator<vertex_id_t>(0), counting_iterator<vertex_id_t>(N), [&](auto i) {
        for (auto j = gr[i].begin(); j != gr[i].end(); ++j) {
          ++degrees[std::get<0>(*j)];
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
      life_timer          _("deserialize");
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

  apb_adj(adj_a);

  return 0;
}
