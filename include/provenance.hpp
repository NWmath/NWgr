//
// This file is part of BGL17 (aka NWGraph aka GraphPack aka the Graph Standard Library)
// (c) Pacific Northwest National Laboratory 2020
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#ifndef NW_GRAPH_PROVENANCE_HPP
#define NW_GRAPH_PROVENANCE_HPP

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace nw {
namespace graph {

std::string& ltrim(std::string& str, const std::string& chars = "\t\n\v\f\r ") {
  str.erase(0, str.find_first_not_of(chars));
  return str;
}

std::string& rtrim(std::string& str, const std::string& chars = "\t\n\v\f\r ") {
  str.erase(str.find_last_not_of(chars) + 1);
  return str;
}

std::string& trim(std::string& str, const std::string& chars = "\t\n\v\f\r ") { return ltrim(rtrim(str, chars), chars); }

class provenance {
public:
  provenance(const provenance&) = default;
  provenance()                  = default;
  provenance(size_t n) : info(n) {}
  void push_back(const std::string& fn, const std::string& str) {
    std::string f(fn), g(str);
    trim(f);
    trim(g);
    info.push_back(fn + ": " + str);
    trim(info.back());
  }

  void push_back(const std::string& str) {
    info.push_back(str);
    trim(info.back());
  }

  void stream(std::ostream& os, const std::string& prefix = "%") const {
    for (auto s : info) {
      os << prefix + " " << s << std::endl;
    }
  }
  auto operator=(const provenance& x) {
    for (auto& j : x.info) {
      info.push_back(j);
    }
  }

private:
  std::vector<std::string> info;
};

}    // namespace graph
}    // namespace nw

#endif    // NW_GRAPH_PROVENANCE_HPP
