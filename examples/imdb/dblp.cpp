
#include <fstream>
#include <iostream>

#include <deque>
#include <map>
#include <queue>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

#include "bfs_edge_range.hpp"
#include "compressed.hpp"
#include "edge_list.hpp"
#include "util/timer.hpp"

std::string delink(const std::string& link) {
  auto opening = link.find("[[");
  if (opening == std::string::npos) {
    return link;
  }
  auto closing = link.find("]]");
  if (opening == std::string::npos) {
    return link + " hm";
  }
  auto delinked = link.substr(opening + 2, closing - opening - 2);
  auto bar      = delinked.find("|");
  if (bar == std::string::npos) {
    return delinked;
  }
  return delinked.substr(bar + 1);
}

int main() {

  std::ifstream ifs("../data/dblp.json");
  //  json jf = json::parse(ifs);

  json jf = json::parse(ifs);

  nw::util::timer t3("build hypergraph");

  std::map<std::string, size_t> titles_map;
  std::map<std::string, size_t> names_map;
  std::vector<std::string>      titles;
  std::vector<std::string>      names;

  nw::graph::edge_list<nw::graph::directed> edges;
  edges.open_for_push_back();

  size_t title_counter = 0;
  size_t name_counter  = 0;

  for (auto& j : jf) {
    auto title   = j[0];
    auto authors = j[1];

    if (titles_map.find(title) == titles_map.end()) {
      titles.emplace_back(title);
      titles_map[title] = titles.size() - 1;
    }

    for (auto& name : authors) {

      if (names_map.find(name) == names_map.end()) {
        names.emplace_back(name);
        names_map[name] = names.size() - 1;
      }

      edges.push_back(titles_map[title], names_map[name]);
    }
  }
  edges.close_for_push_back();

  t3.stop();
  std::cout << t3 << std::endl;
  edges.stream_stats();

  nw::util::timer t4("build biadjacencies");

  auto G = nw::graph::adjacency<0>(edges);
  auto H = nw::graph::adjacency<1>(edges);

  t4.stop();
  std::cout << t4 << std::endl;

  nw::util::timer t5("build s_overlap");

  nw::graph::edge_list<nw::graph::undirected, size_t> s_overlap;
  s_overlap.open_for_push_back();

  for (size_t i = 0; i < H.size(); ++i) {

    if ((i % 8192) == 0) {
      std::cout << i << std::endl;
    }

    for (auto&& [k] : H[i]) {
      for (auto&& [j] : G[k]) {
        if (j > i) {
          s_overlap.push_back(i, j, k);
        }
      }
    }
  }

  s_overlap.close_for_push_back();

  t5.stop();
  std::cout << t5 << std::endl;

  nw::util::timer t6("build s_overlap adjacency");

  auto L = nw::graph::adjacency<0, size_t>(s_overlap);

  t6.stop();
  std::cout << t6 << std::endl;

  size_t kevin_bacon = names_map["Donald E. Knuth"];

  std::vector<size_t> distance(L.size());
  std::vector<size_t> parents(L.size());
  std::vector<size_t> together_in(L.size());

  for (auto&& [u, v, k] : nw::graph::bfs_edge_range(L, kevin_bacon)) {
    distance[v]    = distance[u] + 1;
    parents[v]     = u;
    together_in[v] = k;
  }

  auto path_to_bacon = [&](const std::string& name) {
    size_t the_actor = names_map[name];
    std::cout << name << " has a Knuth number of " << distance[the_actor] << std::endl;

    size_t d = distance[the_actor];
    while (the_actor != kevin_bacon) {
      std::cout << names[the_actor] << " co-authored " << titles[together_in[the_actor]] << " with " << names[parents[the_actor]]
                << std::endl;
      the_actor = parents[the_actor];
      if (d-- == 0) {
        break;
      }
    }
  };

  path_to_bacon("Donald E. Knuth");
  path_to_bacon("Hector Garcia-Molina");
  path_to_bacon("Gene Golub");
  path_to_bacon("Bill Gates");
  path_to_bacon("Jack Dongarra");

  return 0;
}
