//
// This file is part of NWGraph 
// (c) Pacific Northwest National Laboratory 2018-2020
// (c) University of Washington 2018-2020
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//


#ifndef NW_GRAPH_COMPRESSED_HPP
#define NW_GRAPH_COMPRESSED_HPP

#include "adaptors/splittable_range_adapter.hpp"
#include "containers/soa.hpp"
#include "util/defaults.hpp"
#include "util/proxysort.hpp"
#include "util/util.hpp"

#include <algorithm>
#include <concepts>
#include <cxxabi.h>
#include <iostream>
#include <istream>
#include <numeric>

#if defined(CL_SYCL_LANGUAGE_VERSION)
#include <dpstd/algorithm>
#include <dpstd/execution>
#include <dpstd/numeric>
#else
#include <execution>
#endif

#if defined(BGL17_NEED_EXCLUSIVE_SCAN)
#include "detail/numeric.hpp"
#endif

#include <tuple>
#include <vector>

#include "util/defaults.hpp"
#include "util/demangle.hpp"
#include "util/timer.hpp"

namespace nw {
namespace graph {

bool g_debug_compressed = false;
bool g_time_compressed  = false;

void debug_compressed(bool flag = true) { g_debug_compressed = flag; }

void time_compressed(bool flag = true) { g_time_compressed = flag; }

template <typename index_t, typename... Attributes>
class indexed_struct_of_arrays {
  constexpr static const char magic_[34] = "NW GRAPH indexed_struct_of_arrays";

  bool    is_open_ = false;
  index_t N_;

public:    // fixme
  std::vector<index_t>            indices_;
  struct_of_arrays<Attributes...> to_be_indexed_;

  using inner_iterator       = typename struct_of_arrays<Attributes...>::iterator;
  using const_inner_iterator = typename struct_of_arrays<Attributes...>::const_iterator;
  using sub_view             = nw::graph::splittable_range_adapter<inner_iterator>;
  using const_sub_view       = nw::graph::splittable_range_adapter<const_inner_iterator>;

  static constexpr std::size_t getNAttr() { return sizeof...(Attributes); }

  indexed_struct_of_arrays(size_t N) : N_(N), indices_(N + 1) {}
  indexed_struct_of_arrays(size_t N, size_t M) : N_(N), indices_(N + 1), to_be_indexed_(M) {}

  template <std::size_t... Attrs>
  class flat_iterator {
  public:
    using difference_type   = std::ptrdiff_t;
    using value_type        = nw::graph::select_t<std::tuple<index_t, Attributes...>, 0, 1, (Attrs + 2)...>;
    using reference         = nw::graph::select_t<std::tuple<index_t, const Attributes&...>, 0, 1, (Attrs + 2)...>;
    using pointer           = nw::graph::select_t<std::tuple<index_t, const Attributes*...>, 0, 1, (Attrs + 2)...>;
    using iterator_category = std::random_access_iterator_tag;

  private:
    indexed_struct_of_arrays& graph_;    // the underlying indexed data
    index_t                   u_;        // the current source vertex id
    difference_type           j_;        // the current edge

  public:
    flat_iterator(indexed_struct_of_arrays& graph, index_t i, difference_type j) : graph_(graph), u_(i), j_(j) {}

    reference operator*() {
      return {u_, std::get<0>(graph_.to_be_indexed_)[j_], std::get<Attrs + 1>(graph_.to_be_indexed_)[j_]...};
    }

    reference operator[](difference_type n) {
      return {graph_.source(j_ + n), std::get<0>(graph_.to_be_indexed_)[j_ + n], std::get<Attrs + 1>(graph_.to_be_indexed_)[j_]...};
    }

    flat_iterator& operator++() {
      for (++j_; j_ >= graph_.indices_[u_ + 1] && u_ < graph_.indices_.size() - 1; ++u_)
        ;
      return *this;
    }

    flat_iterator& operator--() {
      for (--j_; j_ < graph_.indices_[u_] && u_ > 0; --u_)
        ;
      return *this;
    }

    flat_iterator operator++(int) {
      flat_iterator i = *this;
      ++(*this);
      return i;
    }

    flat_iterator operator--(int) {
      flat_iterator i = *this;
      ++(*this);
      return i;
    }

    flat_iterator& operator+=(difference_type n) {
      j_ += n;
      u_ = graph_.source(j_);
      return *this;
    }

    flat_iterator& operator-=(difference_type n) {
      j_ -= n;
      u_ = graph_.source(j_);
      return *this;
    }

    flat_iterator operator+(difference_type n) const { return {graph_, graph_.source(j_ + n), j_ + n}; }

    flat_iterator operator-(difference_type n) const { return {graph_, graph_.source(j_ - n), j_ - n}; }

    difference_type operator-(const flat_iterator& b) const { return j_ - b.j_; }

    bool operator==(const flat_iterator& b) const { return j_ == b.j_; }

    bool operator!=(const flat_iterator& b) const { return j_ != b.j_; }

    bool operator<(const flat_iterator& b) const { return j_ < b.j_; }

    bool operator>(const flat_iterator& b) const { return j_ > b.j_; }

    bool operator<=(const flat_iterator& b) const { return j_ <= b.j_; }

    bool operator>=(const flat_iterator& b) const { return j_ >= b.j_; }
  };

  /// Provide a tbb split-able range interface to the edge iterators.
  template <std::size_t... Attrs>
  class flat_range {
    flat_iterator<Attrs...> begin_;
    flat_iterator<Attrs...> end_;
    std::ptrdiff_t          cutoff_;

  public:
    flat_range(flat_iterator<Attrs...> begin, flat_iterator<Attrs...> end, std::ptrdiff_t cutoff)
        : begin_(begin), end_(end), cutoff_(cutoff) {}

    flat_range(flat_range& rhs, tbb::split) : begin_(rhs.begin_), end_(rhs.begin_ += rhs.size() / 2), cutoff_(rhs.cutoff_) {}

    flat_iterator<Attrs...> begin() { return begin_; }
    flat_iterator<Attrs...> end() { return end_; }

    std::ptrdiff_t size() const { return end_ - begin_; }
    bool           empty() const { return begin_ == end_; }
    bool           is_divisible() const { return size() >= cutoff_; }
  };

  /// Get a tbb split-able edge range with the passed cutoff.
  template <std::size_t... Attrs>
  flat_range<Attrs...> edges(std::ptrdiff_t cutoff = std::numeric_limits<std::ptrdiff_t>::max()) {
    flat_iterator<Attrs...> begin = {*this, 0, 0};
    flat_iterator<Attrs...> end   = {*this, index_t(indices_.size() - 1), index_t(to_be_indexed_.size())};
    return {begin, end, cutoff};
  }

  class outer_iterator {
    std::vector<index_t>::iterator                     indices_;
    typename struct_of_arrays<Attributes...>::iterator indexed_;
    index_t                                            i_;

  public:
    using difference_type   = index_t;
    using value_type        = sub_view;
    using reference         = value_type&;
    using pointer           = value_type*;
    using iterator_category = std::random_access_iterator_tag;

    outer_iterator(std::vector<index_t>::iterator indices, typename struct_of_arrays<Attributes...>::iterator indexed, index_t i)
        : indices_(indices), indexed_(indexed), i_(i) {}

    outer_iterator& operator++() {
      ++i_;
      return *this;
    }

    outer_iterator& operator+=(difference_type n) {
      i_ += n;
      return *this;
    }

    outer_iterator operator+(difference_type n) const { return {indices_, indexed_, i_ + n}; }
    outer_iterator operator-(difference_type n) const { return {indices_, indexed_, i_ - n}; }

    difference_type operator-(const outer_iterator& b) const { return i_ - b.i_; }

    bool operator==(const outer_iterator& b) const { return i_ == b.i_; }
    bool operator!=(const outer_iterator& b) const { return i_ != b.i_; }
    bool operator<(const outer_iterator& b) const { return i_ < b.i_; }

    value_type operator*() { return {indexed_ + indices_[i_], indexed_ + indices_[i_ + 1]}; }
    value_type operator*() const { return {indexed_ + indices_[i_], indexed_ + indices_[i_ + 1]}; }

    value_type operator[](index_t n) { return {indexed_ + indices_[i_ + n], indexed_ + indices_[i_ + n + 1]}; }

    value_type operator[](index_t n) const { return {indexed_ + indices_[i_ + n], indexed_ + indices_[i_ + n + 1]}; }
  };

  class const_outer_iterator {
    std::vector<index_t>::const_iterator                     indices_;
    typename struct_of_arrays<Attributes...>::const_iterator indexed_;
    index_t                                                  i_;

  public:
    using difference_type   = index_t;
    using value_type        = const_sub_view;
    using reference         = const value_type&;
    using pointer           = const value_type*;
    using iterator_category = std::random_access_iterator_tag;

    const_outer_iterator(std::vector<index_t>::const_iterator                     indices,
                         typename struct_of_arrays<Attributes...>::const_iterator indexed, index_t i)
        : indices_(indices), indexed_(indexed), i_(i) {}

    const_outer_iterator& operator++() {
      ++i_;
      return *this;
    }

    const_outer_iterator& operator+=(difference_type n) {
      i_ += n;
      return *this;
    }

    const_outer_iterator operator+(difference_type n) const { return {indices_, indexed_, i_ + n}; }
    const_outer_iterator operator-(difference_type n) const { return {indices_, indexed_, i_ - n}; }

    difference_type operator-(const const_outer_iterator& b) const { return i_ - b.i_; }

    bool operator==(const const_outer_iterator& b) const { return i_ == b.i_; }
    bool operator!=(const const_outer_iterator& b) const { return i_ != b.i_; }
    bool operator<(const const_outer_iterator& b) const { return i_ < b.i_; }

    value_type operator*() { return {indexed_ + indices_[i_], indexed_ + indices_[i_ + 1]}; }
    value_type operator*() const { return {indexed_ + indices_[i_], indexed_ + indices_[i_ + 1]}; }

    value_type operator[](index_t n) { return {indexed_ + indices_[i_ + n], indexed_ + indices_[i_ + n + 1]}; }

    value_type operator[](index_t n) const { return {indexed_ + indices_[i_ + n], indexed_ + indices_[i_ + n + 1]}; }
  };

  using iterator = outer_iterator;

  using value_type      = typename iterator::value_type;
  using reference       = typename iterator::reference;
  using size_type       = std::size_t;
  using difference_type = typename iterator::difference_type;
  using pointer         = typename iterator::pointer;

  using const_iterator  = const_outer_iterator;
  using const_reference = typename const_iterator::reference;
  using const_pointer   = typename const_iterator::pointer;

  using reverse_iterator       = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  iterator       begin() { return {indices_.begin(), to_be_indexed_.begin(), 0}; }
  const_iterator begin() const { return {indices_.begin(), to_be_indexed_.begin(), 0}; }
  iterator       end() { return {indices_.begin(), to_be_indexed_.begin(), N_}; }
  const_iterator end() const { return {indices_.begin(), to_be_indexed_.begin(), N_}; }

  /// Random access to the outer range.
  sub_view       operator[](index_t i) { return begin()[i]; }
  const_sub_view operator[](index_t i) const { return begin()[i]; }

  index_t size() const { return indices_.size() - 1; }
  // index_t max() const { return indices_.size() - 2; }

  index_t source(difference_type edge) const {
    auto i = std::upper_bound(indices_.begin(), indices_.end(), edge);
    return i - indices_.begin() - 1;
  }

  index_t source(difference_type edge) {
    auto i = std::upper_bound(indices_.begin(), indices_.end(), edge);
    return i - indices_.begin() - 1;
  }

  void open_for_push_back() {
    assert(to_be_indexed_.size() == 0);
    //If we decide to allow reopen for pushback, this will undo exclusive_scan
    /*if(to_be_indexed_.size() != 0) {
      std::adjacent_difference(indices_.begin()+1, indices_.end(), indices_.begin());
      indices_[N_]=0;
      }*/
    is_open_ = true;
  }

  void close_for_push_back() {
    if (to_be_indexed_.size() == 0) return;

    // std::exclusive_scan(std::execution::par, indices_.begin(), indices_.end(), indices_.begin(), 0);

    std::exclusive_scan(indices_.begin(), indices_.end(), indices_.begin(), 0);
    assert(indices_.back() == to_be_indexed_.size());
    is_open_ = false;
  }

  void push_back(index_t i, const Attributes&... attrs) {
    ++indices_[i];
    to_be_indexed_.push_back(attrs...);
  }

  void push_at(index_t i, const Attributes&... attrs) {
    index_t j = indices_[i]++;
    to_be_indexed_.push_at(j, attrs...);
  }

  void stream(const std::string& msg = "") {
    auto first = begin();
    auto last  = end();

    std::cout << msg;
    for (auto G = first; first != last; ++first) {
      for (auto v = (*first).begin(); v != (*first).end(); ++v) {
        std::cout << "( " << first - G << ", " << std::get<0>(*v) << " )" << std::endl;
      }
    }
  }

  void serialize(std::ostream& outfile) {
    size_t el_size = sizeof(indices_[0]);
    size_t st_size = indices_.size();

    outfile.write(reinterpret_cast<const char*>(magic_), sizeof(magic_));
    outfile.write(reinterpret_cast<char*>(&N_), sizeof(size_t));

    outfile.write(reinterpret_cast<char*>(&st_size), sizeof(size_t));
    outfile.write(reinterpret_cast<char*>(&el_size), sizeof(size_t));
    outfile.write(reinterpret_cast<char*>(indices_.data()), st_size * el_size);
    to_be_indexed_.serialize(outfile);
  }

  void serialize(const std::string& outfile_name) {
    std::ofstream out_file(outfile_name, std::ofstream::binary);
    serialize(out_file);
  }

  void deserialize(std::istream& infile) {
    char   spell[sizeof(magic_) + 1];
    size_t el_size = -1;
    size_t st_size = -1;

    infile.read(reinterpret_cast<char*>(spell), sizeof(magic_));
    infile.read(reinterpret_cast<char*>(&N_), sizeof(size_t));

    infile.read(reinterpret_cast<char*>(&st_size), sizeof(size_t));
    infile.read(reinterpret_cast<char*>(&el_size), sizeof(size_t));
    indices_.resize(st_size);
    infile.read(reinterpret_cast<char*>(indices_.data()), st_size * el_size);
    to_be_indexed_.deserialize(infile);
  }

  void deserialize(const std::string& infile_name) {
    std::ifstream infile(infile_name, std::ifstream::binary);
    deserialize(infile);
  }

  template <typename Comparator = decltype(std::less<index_t>{})>
  void triangularize_(Comparator comp = std::less<index_t>{}) {
    std::vector<index_t>            new_indices_(indices_.size());
    struct_of_arrays<Attributes...> new_to_be_indexed_(0);
    new_to_be_indexed_.reserve(to_be_indexed_.size());

    new_indices_[0] = 0;
    for (size_t i = 0; i < N_; ++i) {
      const auto begin = to_be_indexed_.begin() + indices_[i];
      const auto end   = to_be_indexed_.begin() + indices_[i + 1];
      size_t     k     = 0;
      for (auto j = begin; j != end; ++j) {
        auto tmp = std::get<0>(*j);
        if (comp(tmp, i)) {
          new_to_be_indexed_.push_back(*j);
          ++k;
        }
      }
      new_indices_[i + 1] = new_indices_[i] + k;
    }
    indices_       = std::move(new_indices_);
    to_be_indexed_ = std::move(new_to_be_indexed_);
  }

  template <succession cessor>
  void triangularize() {
    if constexpr (cessor == succession::predecessor) {
      triangularize_(std::less<index_t>{});
    } else if constexpr (cessor == succession::successor) {
      triangularize_(std::greater<index_t>{});
    } else {
    }
    if (g_debug_compressed) {
      stream_indices(std::cout);
    }
  }

  std::vector<index_t> degrees() const {
    std::vector<index_t> degrees_(indices_);
    std::adjacent_difference(indices_.begin(), indices_.end(), degrees_.begin());
    return degrees_;
  }

  template <class Graph, class ExecutionPolicy = std::execution::parallel_unsequenced_policy>
  void sort_by_degree(std::string direction = "descending", ExecutionPolicy&& ex_policy = {}) {
    std::vector          degrees_ = degrees();
    std::vector<index_t> perm(indices_.size() - 1);
    std::iota(perm.begin(), perm.end(), 0);
    auto d = degrees_.begin() + 1;

    if (direction == "descending") {
      std::sort(ex_policy, perm.begin(), perm.end(), [&](auto a, auto b) { return d[a] > d[b]; });
    } else if (direction == "ascending") {
      std::sort(ex_policy, perm.begin(), perm.end(), [&](auto a, auto b) { return d[a] < d[b]; });
    } else {
      std::cout << "Unknown direction: " << direction << std::endl;
    }

    std::vector<index_t> new_indices_(indices_);
    auto                 n = new_indices_.begin() + 1;
    std::vector<index_t> iperm(perm.size());

    for (size_t j = 0; j < perm.size(); ++j) {
      n[j]           = d[perm[j]];
      iperm[perm[j]] = j;
    }

    std::inclusive_scan(ex_policy, new_indices_.begin(), new_indices_.end(), new_indices_.begin());

    to_be_indexed_.permute(indices_, new_indices_, perm);
    indices_ = std::move(new_indices_);

    auto b = std::get<0>(to_be_indexed_).begin();

    for (size_t i = 0; i < std::get<0>(to_be_indexed_).size(); ++i) {
      b[i] = iperm[b[i]];
    }

    auto s = std::get<0>(to_be_indexed_).begin();

    for (size_t i = 0; i < perm.size(); ++i) {
      std::sort(ex_policy, s + indices_[i], s + indices_[i + 1]);
    }

    if (g_debug_compressed) {
      stream_indices(std::cout);
    }
  }

  void stream_indices(std::ostream& out = std::cout) {
    auto s = std::get<0>(to_be_indexed_).begin();
    out << "\n+++\n";

    for (size_t i = 0; i < indices_.size() - 1; ++i) {
      out << "==> " << i << ": ";

      for (size_t j = indices_[i]; j < indices_[i + 1]; ++j) {
        out << s[j] << "\t";
      }
      out << std::endl;
    }
    out << "\n+++\n";
  }

  void stream_stats(std::ostream& os = std::cout) const {
    int status = -4;
    std::cout << "% ";
    std::cout << nw::graph::demangle(typeid(*this).name(), nullptr, nullptr, &status);
    std::cout << std::string("indices_.size() ") + std::to_string(indices_.size()) + " ";
    std::cout << std::string("to_be_indexed_.size() ") + std::to_string(to_be_indexed_.size());
    std::cout << std::endl;
  }
};


}    // namespace graph
}    // namespace nw
#endif    // NW_GRAPH_COMPRESSED_HPP
