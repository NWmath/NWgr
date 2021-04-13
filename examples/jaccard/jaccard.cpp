#include "nwgraph/access.hpp"
#include "nwgraph/csr.hpp"
#include "nwgraph/edge_list.hpp"
#include "nwgraph/adaptors/edge_range.hpp"
#include "nwgraph/adaptors/neighbor_range.hpp"
#include "nwgraph/util/intersection_size.hpp"

#include <algorithm>
#include <iostream>
#include <vector>

namespace nw {
namespace graph {
template <class Graph> std::vector<float> jaccard_similarity(Graph& G) {
    using vertex_id_type = vertex_id_t<Graph>;
    vertex_id_type N = num_vertices(G);
    std::vector<vertex_id_type> degrees(N, 0);
    for (auto&& [u, v] : make_edge_range(G)) {
        degrees[u]++;
    }

    std::vector<float> ret(N * N, 0.0);
    for (auto&& [u, u_neighbors] : neighbor_range(G)) {
        for (auto&& [v, v_neighbors] : neighbor_range(G)) {
            size_t intersect_size = intersection_size(u_neighbors, v_neighbors);
            std::cout << "( " << u << ", " << v << "): intersect = " << intersect_size
                      << " deg(u) = " << degrees[u] << " deg(v) = " << degrees[v] << std::endl;
            float jaccard = ((float)intersect_size) / (degrees[u] + degrees[v] - intersect_size);
            // Special case self-intersections
            ret[u + N * v] = (intersect_size != 0) ? jaccard : (u == v ? 1.0 : 0.0);
        }
    }
    return ret;
}

template<class Graph> Graph transpose(Graph& G) {
    edge_list<nw::graph::directedness::directed> ret;
    ret.open_for_push_back();
    for (auto&& [u, v] : make_edge_range(G)) {
        ret.push_back(v, u);
    }
    ret.close_for_push_back();
    return Graph(ret);
}

template<class Adj> std::vector<float> jaccard_sparse(Adj& G) {
    using vertex_id_type = vertex_id_t<Adj>;
    vertex_id_type N = num_vertices(G);
    std::vector<vertex_id_type> degrees(N, 0);
    for (auto&& [u, v] : make_edge_range(G)) {
        degrees[u]++;
    }

    Adj G_t = transpose(G);
    std::vector<float> ret(N * N, 0.0);
    for (auto&& [u, v] : make_edge_range(G)) {
        for (auto&& [w] : G_t[v]) {
            size_t intersect_size = intersection_size(G[u], G[w]);
            std::cout << "( " << u << ", " << w << "): intersect = " << intersect_size
                      << " deg(u) = " << degrees[u] << " deg(w) = " << degrees[w] << std::endl;
            float jaccard = ((float)intersect_size) / (degrees[u] + degrees[w] - intersect_size);
            // Special case self-intersections
            ret[u + N * w] = (intersect_size != 0) ? jaccard : (u == v ? 1.0 : 0.0);

        }
    }
    // Patch up self-intersections
    for (vertex_id_type i = 0; i < N; i++) {
        ret[i + N * i] = 1.0;
    }
    return ret;
}
}
}

bool test() {
    using namespace nw::graph;
    edge_list<nw::graph::directedness::directed> data;
    data.open_for_push_back();
    // It was very much unclear that, when I was defining an undirected graph, I needed to put both edges in

    // Also, might it be good to standardize what the iterators on graphs return?
    // I hit all matter of template errors.

    data.push_back(0, 1);
    data.push_back(2, 1);
    data.close_for_push_back();

    csr_graph sparse_data(data);
    size_t N = num_vertices(sparse_data);
    auto csr_jaccard = jaccard_similarity(sparse_data);

    for (size_t i = 0; i < N; i++) {
      for (size_t j = 0; j < N; j++) {
          std::cout << csr_jaccard[j + N * i];
          if (j != N - 1) {
              std::cout << " ";
          }
      }
      std::cout << std::endl;
    }

    auto jaccard_by_transpose = jaccard_sparse(sparse_data);
    return
        std::equal(csr_jaccard.cbegin(), csr_jaccard.cend(),
                   jaccard_by_transpose.cbegin(), jaccard_by_transpose.cend(),
                   [](const float &a, const float &b) { return std::abs(a - b) < 1e-5; } );
}

int main() {
    bool test_result = test();
    if (!test_result) {
        std::cerr << "Jaccard varries with algorithm somehow" << std::endl;
    }
    return 0;
}
