/**
 * @file util_par.hpp
 *
 * @copyright SPDX-FileCopyrightText: 2022 Batelle Memorial Institute
 * @copyright SPDX-FileCopyrightText: 2022 University of Washington
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * @authors
 *   Andrew Lumsdaine
 *
 */

// 
// This file is part of NW Graph (aka GraphPack) 
// (c) Pacific Northwest National Laboratory 2018-2021 
// (c) University of Washington 2018-2021 
// 
// Licensed under terms of include LICENSE file 
// 
// Authors: 
//     Andrew Lumsdaine	
//     Kevin Deweese	
//

#ifndef NW_GRAPH_UTIL_PAR_HPP
#define NW_GRAPH_UTIL_PAR_HPP

#include <atomic>
#include <cstddef>
#include <iterator>
#include <mutex>

namespace nw {
namespace graph {

std::mutex mtx;

class par_counting_output_iterator : public std::iterator<std::random_access_iterator_tag, size_t> {
public:
  par_counting_output_iterator(size_t& count) : count{count} {}
  void                          operator++() {}
  void                          operator++(int) {}
  par_counting_output_iterator& operator*() { return *this; }
  par_counting_output_iterator& operator[](size_t) { return *this; }

  template <typename T>
  void operator=(T) {
    std::lock_guard<std::mutex> lock(mtx);
    count++;
  }
  size_t get_count() { return count; }

private:
  size_t& count;
};

class atomic_counting_output_iterator : public std::iterator<std::random_access_iterator_tag, size_t> {
public:
  atomic_counting_output_iterator(std::atomic<size_t>& count) : count{count} {}
  void                             operator++() {}
  void                             operator++(int) {}
  atomic_counting_output_iterator& operator*() { return *this; }
  atomic_counting_output_iterator& operator[](size_t) { return *this; }

  template <typename T>
  void operator=(T) {
    count++;
  }
  size_t get_count() { return count; }

private:
  std::atomic<size_t>& count;
};

typedef size_t vertex_id_type;

}    // namespace graph
}    // namespace nw

#endif    // NW_GRAPH_UTIL_PAR_HPP
