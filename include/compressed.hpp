

#ifndef NW_GRAPH_COMPRESSED_HPP
#define NW_GRAPH_COMPRESSED_HPP

#include "aos.hpp"
#include "edge_list.hpp"
#include "graph_base.hpp"
#include "proxysort.hpp"
#include "soa.hpp"
#include "splittable_range_adapter.hpp"
#include "util/util.hpp"
#include "util/types.hpp"

#include <algorithm>
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

#include "util/demangle.hpp"
#include "util/timer.hpp"
#include "util/types.hpp"

namespace nw {
namespace graph {

bool g_debug_compressed = false;
bool g_time_compressed  = false;

void debug_compressed(bool flag = true) { g_debug_compressed = flag; }

void time_compressed(bool flag = true) { g_time_compressed = flag; }

template <typename... Attributes>
class indexed_struct_of_arrays {
  constexpr static const char magic_[24] = "BGL17 compressed_sparse";

  bool        is_open_ = false;
  vertex_id_t N_;

public:    // fixme
  std::vector<vertex_id_t>        indices_;
  struct_of_arrays<Attributes...> to_be_indexed_;

  using edge_id_t      = std::ptrdiff_t;
  using inner_iterator = typename struct_of_arrays<Attributes...>::iterator;
  using sub_view       = nw::graph::splittable_range_adapter<inner_iterator>;

  static constexpr std::size_t getNAttr() { return sizeof...(Attributes); }

  indexed_struct_of_arrays(size_t N) : N_(N), indices_(N + 1) {}
  indexed_struct_of_arrays(size_t N, size_t M) : N_(N), indices_(N + 1), to_be_indexed_(M) {}

  /// A linear edge iterator that supports random-access operations.
  ///
  /// This edge iterator currently only iterates edges as u,v
  /// pairs. Bidirectional iterator operations are very efficient, while random
  /// access operations like `+=` require a log2 search of the index array.
  ///
  /// @tparam  Attrs... The 0-based indices of the edge attributes to access.
  template <std::size_t... Attrs>
  class edge_iterator {
  public:
    using difference_type   = edge_id_t;
    using value_type        = nw::graph::select_t<std::tuple<vertex_id_t, Attributes...>, 0, 1, (Attrs + 2)...>;
    using reference         = nw::graph::select_t<std::tuple<vertex_id_t, const Attributes&...>, 0, 1, (Attrs + 2)...>;
    using pointer           = nw::graph::select_t<std::tuple<vertex_id_t, const Attributes*...>, 0, 1, (Attrs + 2)...>;
    using iterator_category = std::random_access_iterator_tag;

  private:
    indexed_struct_of_arrays& graph_;    // the underlying indexed data
    vertex_id_t               u_;        // the current source vertex id
    edge_id_t                 j_;        // the current edge

  public:
    edge_iterator(indexed_struct_of_arrays& graph, vertex_id_t i, edge_id_t j) : graph_(graph), u_(i), j_(j) {}

    reference operator*() {
      return {u_, std::get<0>(graph_.to_be_indexed_)[j_], std::get<Attrs + 1>(graph_.to_be_indexed_)[j_]...};
    }

    reference operator[](edge_id_t n) {
      return {graph_.source(j_ + n), std::get<0>(graph_.to_be_indexed_)[j_ + n], std::get<Attrs + 1>(graph_.to_be_indexed_)[j_]...};
    }

    edge_iterator& operator++() {
      for (++j_; j_ >= graph_.indices_[u_ + 1] && u_ < graph_.indices_.size() - 1; ++u_)
        ;
      return *this;
    }

    edge_iterator& operator--() {
      for (--j_; j_ < graph_.indices_[u_] && u_ > 0; --u_)
        ;
      return *this;
    }

    edge_iterator operator++(int) {
      edge_iterator i = *this;
      ++(*this);
      return i;
    }

    edge_iterator operator--(int) {
      edge_iterator i = *this;
      ++(*this);
      return i;
    }

    edge_iterator& operator+=(edge_id_t n) {
      j_ += n;
      u_ = graph_.source(j_);
      return *this;
    }

    edge_iterator& operator-=(edge_id_t n) {
      j_ -= n;
      u_ = graph_.source(j_);
      return *this;
    }

    edge_iterator operator+(edge_id_t n) const { return {graph_, graph_.source(j_ + n), j_ + n}; }

    edge_iterator operator-(edge_id_t n) const { return {graph_, graph_.source(j_ - n), j_ - n}; }

    edge_id_t operator-(const edge_iterator& b) const { return j_ - b.j_; }

    bool operator==(const edge_iterator& b) const { return j_ == b.j_; }

    bool operator!=(const edge_iterator& b) const { return j_ != b.j_; }

    bool operator<(const edge_iterator& b) const { return j_ < b.j_; }

    bool operator>(const edge_iterator& b) const { return j_ > b.j_; }

    bool operator<=(const edge_iterator& b) const { return j_ <= b.j_; }

    bool operator>=(const edge_iterator& b) const { return j_ >= b.j_; }
  };

  /// Provide a tbb split-able range interface to the edge iterators.
  template <std::size_t... Attrs>
  class edge_range {
    edge_iterator<Attrs...> begin_;
    edge_iterator<Attrs...> end_;
    std::ptrdiff_t          cutoff_;

  public:
    edge_range(edge_iterator<Attrs...> begin, edge_iterator<Attrs...> end, std::ptrdiff_t cutoff)
        : begin_(begin), end_(end), cutoff_(cutoff) {}

    edge_range(edge_range& rhs, tbb::split) : begin_(rhs.begin_), end_(rhs.begin_ += rhs.size() / 2), cutoff_(rhs.cutoff_) {}

    edge_iterator<Attrs...> begin() { return begin_; }
    edge_iterator<Attrs...> end() { return end_; }

    std::ptrdiff_t size() const { return end_ - begin_; }
    bool           empty() const { return begin_ == end_; }
    bool           is_divisible() const { return size() >= cutoff_; }
  };

  /// Get a tbb split-able edge range with the passed cutoff.
  template <std::size_t... Attrs>
  edge_range<Attrs...> edges(std::ptrdiff_t cutoff = std::numeric_limits<std::ptrdiff_t>::max()) {
    edge_iterator<Attrs...> begin = {*this, 0, 0};
    edge_iterator<Attrs...> end   = {*this, vertex_id_t(indices_.size() - 1), vertex_id_t(to_be_indexed_.size())};
    return {begin, end, cutoff};
  }

  /// This iterator provides a 2D vertex-neighbor tbb split-able interface to
  /// the graph.
  class outer_iterator {
    std::vector<vertex_id_t>::iterator                 indices_;
    typename struct_of_arrays<Attributes...>::iterator indexed_;
    vertex_id_t                                        i_;

  public:
    using difference_type   = vertex_id_t;
    using value_type        = sub_view;
    using reference         = value_type&;
    using pointer           = value_type*;
    using iterator_category = std::random_access_iterator_tag;

    outer_iterator(std::vector<vertex_id_t>::iterator indices, typename struct_of_arrays<Attributes...>::iterator indexed,
                   vertex_id_t i)
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

    value_type operator[](vertex_id_t n) { return {indexed_ + indices_[i_ + n], indexed_ + indices_[i_ + n + 1]}; }

    value_type operator[](vertex_id_t n) const { return {indexed_ + indices_[i_ + n], indexed_ + indices_[i_ + n + 1]}; }
  };
  using iterator = outer_iterator;

  iterator begin() { return {indices_.begin(), to_be_indexed_.begin(), 0}; }
  iterator begin() const { return {indices_.begin(), to_be_indexed_.begin(), 0}; }
  iterator end() { return {indices_.begin(), to_be_indexed_.begin(), N_}; }
  iterator end() const { return {indices_.begin(), to_be_indexed_.begin(), N_}; }

  /// Random access to the outer range.
  sub_view operator[](vertex_id_t i) { return begin()[i]; }
  sub_view operator[](vertex_id_t i) const { return begin()[i]; }

  vertex_id_t size() const { return indices_.size() - 1; }
  vertex_id_t max() const { return indices_.size() - 2; }

  vertex_id_t source(edge_id_t edge) const {
    auto i = std::upper_bound(indices_.begin(), indices_.end(), edge);
    return i - indices_.begin() - 1;
  }

  vertex_id_t source(edge_id_t edge) {
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
    //std::exclusive_scan(std::execution::par, indices_.begin(), indices_.end(), indices_.begin(), 0);
    std::exclusive_scan(indices_.begin(), indices_.end(), indices_.begin(), 0);
    assert(indices_.back() == to_be_indexed_.size());
    is_open_ = false;
  }

  void push_back(vertex_id_t i, const Attributes&... attrs) {
    ++indices_[i];
    to_be_indexed_.push_back(attrs...);
  }

  void push_at(vertex_id_t i, const Attributes&... attrs) {
    vertex_id_t j = indices_[i]++;
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

  template <typename Comparator = decltype(std::less<vertex_id_t>{})>
  void triangularize_(Comparator comp = std::less<vertex_id_t>{}) {
    std::vector<vertex_id_t>        new_indices_(indices_.size());
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
    if constexpr (cessor == predecessor) {
      triangularize_(std::less<vertex_id_t>{});
    } else if constexpr (cessor == successor) {
      triangularize_(std::greater<vertex_id_t>{});
    } else {
    }
    if (g_debug_compressed) {
      stream_indices(std::cout);
    }
  }

  std::vector<vertex_id_t> degrees() const {
    std::vector<vertex_id_t> degrees_(indices_);
    std::adjacent_difference(indices_.begin(), indices_.end(), degrees_.begin());
    return degrees_;
  }

  template <class Graph, class ExecutionPolicy = std::execution::parallel_unsequenced_policy>
  void sort_by_degree(std::string direction = "descending", ExecutionPolicy&& ex_policy = {}) {
    std::vector              degrees_ = degrees();
    std::vector<vertex_id_t> perm(indices_.size() - 1);
    std::iota(perm.begin(), perm.end(), 0);
    auto d = degrees_.begin() + 1;

    if (direction == "descending") {
      std::sort(ex_policy, perm.begin(), perm.end(), [&](auto a, auto b) { return d[a] > d[b]; });
    } else if (direction == "ascending") {
      std::sort(ex_policy, perm.begin(), perm.end(), [&](auto a, auto b) { return d[a] < d[b]; });
    } else {
      std::cout << "Unknown direction: " << direction << std::endl;
    }

    std::vector<vertex_id_t> new_indices_(indices_);
    auto                     n = new_indices_.begin() + 1;
    std::vector<vertex_id_t> iperm(perm.size());

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

/**
 * @brief
 *
 * @todo compressed_sparse should be deprecated -- there is no such thing as directedness
 *
 * @file compressed.hpp
 * @author Andrew Lumsdaine
 * @date 2018-08-22
 */

template <directedness edge_directedness, typename... Attributes>
class edge_list;

template <int idx = 0, directedness sym = undirected, typename... Attributes>
class compressed_sparse : public indexed_struct_of_arrays<vertex_id_t, Attributes...> {
public:
  // The first vertex_id_t isn't considered an attribute.
  using attributes_t = std::tuple<Attributes...>;
  static constexpr std::size_t getNAttr() { return sizeof...(Attributes); }

  compressed_sparse(size_t N) : indexed_struct_of_arrays<vertex_id_t, Attributes...>(N) {}
  compressed_sparse(edge_list<sym, Attributes...>& A) : indexed_struct_of_arrays<vertex_id_t, Attributes...>(A.max()[idx] + 1) {
    A.fill(*this);
  }

  //  size_t size() const { return indexed_struct_of_arrays<vertex_id_t, Attributes...>::size(); }
};

template <int idx, typename... Attributes>
class adjacency : public indexed_struct_of_arrays<vertex_id_t, Attributes...> {
public:
  // The first vertex_id_t isn't considered an attribute.
  using attributes_t = std::tuple<Attributes...>;
  static constexpr std::size_t getNAttr() { return sizeof...(Attributes); }
  using vertex_id_t = nw::graph::vertex_id_t;

  adjacency(size_t N = 0, size_t M = 0) : indexed_struct_of_arrays<vertex_id_t, Attributes...>(N, M) {}
  template <class ExecutionPolicy = std::execution::parallel_unsequenced_policy>
  adjacency(edge_list<directed, Attributes...>& A, ExecutionPolicy&& policy = {}) : indexed_struct_of_arrays<vertex_id_t, Attributes...>(std::max(A.max()[0], A.max()[1]) + 1) {
    A.fill(*this, policy);
  }
  template <class ExecutionPolicy = std::execution::parallel_unsequenced_policy>
  adjacency(edge_list<undirected, Attributes...>& A, ExecutionPolicy&& policy = {})
      : indexed_struct_of_arrays<vertex_id_t, Attributes...>(std::max(A.max()[0], A.max()[1]) + 1) {
    A.fill(*this, policy);
  }

  //  size_t size() const { return indexed_struct_of_arrays<vertex_id_t, Attributes...>::size(); }

#if 0
  auto transpose() {
    if ((idx != 0) || (idx != 1)) {
      throw;
    }
    size_t t_idx = (idx+1) % 2;
    adjacency<t_idx, Attributes...> A_transpose(N);
    auto A_column_nos = std::get<0>(to_be_indexed_);
    auto A_row_indices = indices_;

    auto A_t_col_indices = A_transpose.indices_;

    for (size_t i = 0; size_t < A_columns.size(); ++i) {
      auto col = A_column_nos[i];
      A_t_col_indices[col]++;
    }
    A_transpose.close_for_push_back();
    A_transpose.resize(to_be_indexed_.size());

    auto A_t_row_nos = std::get<0>(A_transpose.to_be_indexed_);

    for (size_t i = 0; i < N_; ++i) {
      for (size_t j = indices_[i]; j < indices_[i+1]; ++j) {
    auto_col = A_column_nos[j];
    auto A_tra = A_t_col_indices_[A_col]++;
    A_transpose.to_be_indexed_[A_tra] = to_be_indexed_[j];
    A_t_row_nos[A_tra] = i;
      }
    }
    return A_transpose;
  }
#endif
};

template <int idx, succession cessor, typename... Attributes>
class packed : public indexed_struct_of_arrays<vertex_id_t, Attributes...> {
public:
  // The first vertex_id_t isn't considered an attribute.
  using attributes_t = std::tuple<Attributes...>;
  static constexpr std::size_t getNAttr() { return sizeof...(Attributes); }

  packed(size_t N) : indexed_struct_of_arrays<vertex_id_t, Attributes...>(N) {}
  packed(edge_list<undirected, Attributes...>& A) : indexed_struct_of_arrays<vertex_id_t, Attributes...>(A.size()) {
    A.fill(*this);
  }

  //  size_t size() const { return indexed_struct_of_arrays<vertex_id_t, Attributes...>::size(); }
};

}    // namespace graph
}    // namespace nw
#endif    // NW_GRAPH_COMPRESSED_HPP
