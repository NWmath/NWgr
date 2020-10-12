
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
  auto delinked = link.substr(opening + 2, closing-opening-2);
  auto bar = delinked.find("|");
  if (bar == std::string::npos) {
    return delinked;
  }
  return delinked.substr(bar+1);
}


int main() {

  std::ifstream ifs("../data/oracle.json");
  //  json jf = json::parse(ifs);

  std::vector<json> jsons;
  while (!ifs.eof() && ifs.peek() != EOF) {


    std::string str;
    std::getline(ifs, str);
    json jf = json::parse(str);

    jsons.emplace_back(jf);

  }
    
  nw::util::timer t3("build hypergraph");

  std::map<std::string, size_t> titles_map;
  std::map<std::string, size_t> names_map;
  std::vector<std::string> titles;
  std::vector<std::string> names;

  nw::graph::edge_list<nw::graph::directed> edges;
  edges.open_for_push_back();

  size_t title_counter = 0;
  size_t name_counter  = 0;

  for (auto& j : jsons) {
    auto title = j["title"];
    
    if (titles_map.find(title) == titles_map.end()) {
      titles.emplace_back(title);
      titles_map[title] = titles.size()-1;
    }
    
    for (auto& k : j["cast"]) {
      auto name = delink(k);
      
      if (names_map.find(name) == names_map.end()) {
	names.emplace_back(name);
	names_map[name] = names.size()-1;
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

  // Kevin Bacon is nm0000102
  // David Suchet is nm0837064
  // Kyra Sedgwick is nm0001718

  size_t kevin_bacon   = names_map["Kevin Bacon"];
  size_t david_suchet  = names_map["David Suchet"];
  size_t kyra_sedgwick = names_map["Kyra Sedgwick"];

  std::vector<size_t> distance(L.size());
  std::vector<size_t> parents(L.size());
  std::vector<size_t> together_in(L.size());

  for (auto&& [u, v, k] : nw::graph::bfs_edge_range(L, kevin_bacon)) {
    distance[v]    = distance[u] + 1;
    parents[v]     = u;
    together_in[v] = k;
  }

  std::cout << "Kevin Bacon has a Bacon number of " << distance[kevin_bacon] << std::endl;

  std::cout << "Kyra Sedgwick has a bacon number of " << distance[kyra_sedgwick] << std::endl;
  size_t d = distance[kyra_sedgwick];
  while (kyra_sedgwick != kevin_bacon) {
    std::cout << names[kyra_sedgwick] << " starred with " << names[parents[kyra_sedgwick]] << " in "
              << titles[together_in[kyra_sedgwick]] << std::endl;
    kyra_sedgwick = parents[kyra_sedgwick];
    if (d-- == 0) {
      break;
    }
  }

  std::cout << "David Suchet has a Bacon number of " << distance[david_suchet] << std::endl;
  d = distance[david_suchet];
  while (david_suchet != kevin_bacon) {
    std::cout << names[david_suchet] << " starred with " << names[parents[david_suchet]] << " in "
              << titles[together_in[david_suchet]] << std::endl;
    david_suchet = parents[david_suchet];
    if (d-- == 0) {
      break;
    }
  }


  return 0;
}
