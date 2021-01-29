

#ifndef NW_GRAPH_SPLITTABLE_RANGE_ADAPTER_HPP
#define NW_GRAPH_SPLITTABLE_RANGE_ADAPTER_HPP

#include <cstddef>
#include <tbb/tbb_stddef.h>

namespace nw {
namespace graph {
template <class Iterator>
class splittable_range_adapter {
  Iterator    begin_;
  Iterator    end_;
  std::size_t cutoff_ = 16384;

public:
  using iterator = Iterator;
  using value_type = iterator::value_type;

  template <class Range>
  explicit splittable_range_adapter(Range&& range) : splittable_range_adapter(range.begin(), range.end()) {}

  template <class Range>
  splittable_range_adapter(Range&& range, std::size_t cutoff) : splittable_range_adapter(range.begin(), range.end(), cutoff) {}

  splittable_range_adapter(Iterator begin, Iterator end) : begin_(begin > end ? end : begin), end_(end) {}

  splittable_range_adapter(Iterator begin, Iterator end, std::size_t cutoff)
      : begin_(begin > end ? end : begin), end_(end), cutoff_(cutoff) {}

  splittable_range_adapter(splittable_range_adapter& rhs, tbb::split)
      : begin_(rhs.begin_), end_(rhs.begin_ += rhs.size() / 2), cutoff_(rhs.cutoff_) {}

  // We need the weird ref version to disambiguate the explicit range
  // initializer, which would otherwise get called incorrectly during a tbb
  // split event.
  splittable_range_adapter(splittable_range_adapter&)       = default;
  splittable_range_adapter(const splittable_range_adapter&) = default;
  splittable_range_adapter(splittable_range_adapter&&)      = default;

  decltype(auto) begin() { return begin_; }
  decltype(auto) end() { return end_; }

  decltype(auto) cutoff(std::size_t cutoff) {
    cutoff_ = cutoff;
    return *this;
  }

  size_t size() const { return end_ - begin_; }
  bool   empty() const { return size() == 0; }
  bool   is_divisible() const { return size() > cutoff_; }
};

template <class Range>
splittable_range_adapter(Range&& range) -> splittable_range_adapter<decltype(range.begin())>;

template <class Range>
splittable_range_adapter(Range&& range, std::size_t) -> splittable_range_adapter<decltype(range.begin())>;

}    // namespace graph
}    // namespace nw

#endif    // NW_GRAPH_SPLITTABLE_RANGE_ADAPTER_HPP
