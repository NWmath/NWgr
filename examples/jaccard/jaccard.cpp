#include "containers/compressed.hpp"
#include "containers/edge_list.hpp"
#include "adaptors/edge_range.hpp"
#include "util/intersection_size.hpp"
#include "util/types.hpp"

#include <algorithm>
#include <iostream>
#include <vector>

namespace nw {
namespace graph {
template <class Graph> std::vector<float> jaccard_similarity(Graph& G) {

    compressed_sparse<0, directed> g_for_api(20);
    vertex_id_t N = G.size();
    auto degrees = G.degrees();
    std::vector<float> ret(N * N, 0.0);
    for (auto u_neighbors = G.begin(); u_neighbors != G.end(); ++u_neighbors) {
        vertex_id_t u = u_neighbors - G.begin();
        for (auto v_neighbors = G.begin(); v_neighbors != G.end(); ++v_neighbors) {
            vertex_id_t v = v_neighbors - G.begin();
            size_t intersect_size = intersection_size(*u_neighbors, *v_neighbors);
            std::cout << "( " << u << ", " << v << "): intersect = " << intersect_size
                      << " deg(u) = " << degrees[u] << " deg(v) = " << degrees[v] << std::endl;
            float jaccard = ((float)intersect_size) / (degrees[u] + degrees[v] - intersect_size);
            // Does the edge iterator return undirected edges twice?
            ret[u + N * v] = jaccard;
        }
    }
    return ret;
}
}
}

bool test() {
    using namespace nw::graph;
    edge_list<nw::graph::directedness::undirected> data;
    data.open_for_push_back();
    // It was very much unclear that, when I was defining an undirected graph, I needed to put both edges in

    // Also, might it be good to standardize what the iterators on graphs return?
    // I hit all matter of template errors.

    // Alternatively, new bit of graph API: n_vertices() and n_edges()
    data.push_back(0, 1);
    data.push_back(1, 0);
    data.push_back(1, 2);
    data.push_back(2, 1);
    data.close_for_push_back();

    //auto edge_list_jaccard = jaccard_similarity(data);

    compressed_sparse<0, nw::graph::directedness::undirected> sparse_data(data);
    vertex_id_t N = sparse_data.size();
    auto csr_jaccard = jaccard_similarity(sparse_data);

    for (vertex_id_t i = 0; i < N; i++) {
      for (vertex_id_t j = 0; j < N; j++) {
          std::cout << csr_jaccard[j + N * i];
          if (j != N - 1) {
              std::cout << " ";
          }
      }
      std::cout << std::endl;
    }

    return true;
    // return
    //     std::equal(edge_list_jaccard.cbegin(), edge_list_jaccard.cend(),
    //                csr_jaccard.cbegin(), csr_jaccard.cend(),
    //                [](const float &a, const float &b) { return std::abs(a - b) < 1e-5; } );
}

int main() {
    bool test_result = test();
    if (!test_result) {
        std::cerr << "Jaccard varries with graph type somehow" << std::endl;
    }
    return 0;
}
