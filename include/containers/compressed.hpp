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
#include "graph_base.hpp"
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

#include "util/arrow_proxy.hpp"
#include "util/defaults.hpp"
#include "util/demangle.hpp"
#include "util/timer.hpp"

#include "access.hpp"


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

  template <bool is_const = false>
  class my_outer_iterator {
   public:
    using   const_index_iterator_t = typename std::vector<index_t>::const_iterator;
    using         index_iterator_t = typename std::vector<index_t>::iterator;
    using const_indexed_iterator_t = typename struct_of_arrays<Attributes...>::const_iterator;
    using       indexed_iterator_t = typename struct_of_arrays<Attributes...>::iterator;

   private:
    friend class my_outer_iterator<!is_const>;
    using   index_it_t = std::conditional_t<is_const, const_index_iterator_t, index_iterator_t>;
    using indexed_it_t = std::conditional_t<is_const, const_indexed_iterator_t, indexed_iterator_t>;

    index_it_t   indices_;
    indexed_it_t indexed_;
    index_t            i_;

   public:
    using difference_type   = std::make_signed_t<index_t>;
    using value_type        = std::conditional_t<is_const, const_sub_view, sub_view>;
    using reference         = value_type;
    using pointer           = arrow_proxy<reference>;
    using iterator_category = std::random_access_iterator_tag;

    my_outer_iterator() = default;

    my_outer_iterator(index_iterator_t indices, indexed_iterator_t indexed, index_t i) requires(is_const)
        : indices_(indices)
        , indexed_(indexed)
        , i_(i)
    {
    }

    my_outer_iterator(index_it_t indices, indexed_it_t indexed, index_t i)
        : indices_(indices)
        , indexed_(indexed)
        , i_(i)
    {
    }

    my_outer_iterator(const my_outer_iterator&) = default;
    my_outer_iterator(const my_outer_iterator<false>& rhs) requires(is_const)
        : indices_(rhs.indices_)
        , indexed_(rhs.indexed_)
        , i_(rhs.i_)
    {
    }

    my_outer_iterator& operator=(const my_outer_iterator&) = default;
    my_outer_iterator& operator=(const my_outer_iterator<false>& rhs) requires(is_const)
    {
      indices_ = rhs.indices_;
      indexed_ = rhs.indexed_;
      i_       = rhs.i_;
      return *this;
    }

    my_outer_iterator& operator++() {
      ++i_;
      return *this;
    }

    my_outer_iterator operator++(int) const {
      my_outer_iterator tmp(*this);
      ++i_;
      return tmp;
      ;
    }

    my_outer_iterator& operator--() {
      --i_;
      return *this;
    }

    my_outer_iterator operator--(int) const {
      my_outer_iterator tmp(*this);
      --i_;
      return tmp;
    }

    my_outer_iterator& operator+=(difference_type n) {
      i_ += n;
      return *this;
    }

    my_outer_iterator& operator-=(difference_type n) {
      i_ -= n;
      return *this;
    }

    my_outer_iterator operator+(difference_type n) const {
      return { indices_, indexed_, i_ + n };
    }

    my_outer_iterator operator-(difference_type n) const { return {indices_, indexed_, i_ - n}; }

    difference_type operator-(const my_outer_iterator& b) const { return i_ - b.i_; }

    bool operator==(const my_outer_iterator& b) const { return i_ == b.i_; }
    bool operator!=(const my_outer_iterator& b) const { return i_ != b.i_; }
    bool operator<(const my_outer_iterator& b) const { return i_ < b.i_; }
    bool operator>(const my_outer_iterator& b) const { return i_ > b.i_; }
    bool operator<=(const my_outer_iterator& b) const { return i_ <= b.i_; }
    bool operator>=(const my_outer_iterator& b) const { return i_ >= b.i_; }

    reference operator*() { return {indexed_ + indices_[i_], indexed_ + indices_[i_ + 1]}; }
    reference operator*() const { return {indexed_ + indices_[i_], indexed_ + indices_[i_ + 1]}; }

    pointer operator->() { return {**this}; }
    pointer operator->() const { return {**this}; }

    reference operator[](index_t n) { return {indexed_ + indices_[i_ + n], indexed_ + indices_[i_ + n + 1]}; }
    reference operator[](index_t n) const { return {indexed_ + indices_[i_ + n], indexed_ + indices_[i_ + n + 1]}; }
  };

  using const_outer_iterator = my_outer_iterator<true>;
  using outer_iterator = my_outer_iterator<false>;

  using iterator = my_outer_iterator<false>;

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

template <typename index_t, typename... Attributes>
auto operator+(typename std::iter_difference_t<typename indexed_struct_of_arrays<index_t, Attributes...>::outer_iterator> n, const typename indexed_struct_of_arrays<index_t, Attributes...>::outer_iterator i) {
  return i + n;
}

template <std::signed_integral T, typename I>
I operator+(T n, const I i) {
  return i + n;
}




}    // namespace graph
}    // namespace nw
#endif    // NW_GRAPH_COMPRESSED_HPP
