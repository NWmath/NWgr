#pragma once

#include <iterator>

namespace bgl17 {
/// This class template applies a permutation to a random-access iterator.
///
/// @tparam    Iterator The underlying random access iterator type.
/// @tparam Permutation The permutation provider class.
template <class Iterator, class Permutation>
class permutation_iterator
{
  static_assert(std::is_base_of_v<std::random_access_iterator_tag, typename std::iterator_traits<Iterator>::iterator_category>);

 public:
  using   difference_type = typename std::iterator_traits<Iterator>::difference_type;
  using        value_type = typename std::iterator_traits<Iterator>::value_type;
  using         reference = typename std::iterator_traits<Iterator>::reference;
  using           pointer = typename std::iterator_traits<Iterator>::pointer;
  using iterator_category = std::input_iterator_tag;

 private:
  Iterator     base_;
  Permutation  perm_;
  difference_type i_;

 public:
  template <class Index>
  permutation_iterator(Iterator base, Index i, Permutation perm) : base_(base), perm_(perm), i_(i) {
  }

  constexpr decltype(auto) operator*() {
    return *(base_ + perm_(i_));
  }

  permutation_iterator& operator++() {
    ++i_;
    return *this;
  }

  constexpr bool operator!=(const permutation_iterator& rhs) {
    return (i_ != rhs.i_ || base_ != rhs.base_);
  }
};
}
