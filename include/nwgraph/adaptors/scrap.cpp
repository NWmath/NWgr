

template <typename Graph>
class plain_range {

public:
  plain_range(const Graph& g) : the_graph_(g) {}

  class iterator {
    typename Graph::iterator base_;
    typename Graph::iterator first_;

  public:
    iterator(typename Graph::iterator base, typename Graph::iterator first) : base_(base), first_(first) {}

    iterator& operator++() {
      ++first_;
      return *this;
    }

    auto operator*() { return std::tuple(first_ - base_); }

    bool operator==(const iterator& b) const { return first_ == b.first_; }
    bool operator!=(const iterator& b) const { return first_ != b.first_; }
  };

  class const_iterator {
    typename Graph::iterator base_;
    typename Graph::iterator first_;

  public:
    const_iterator(typename Graph::iterator base, typename Graph::iterator first) : base_(base), first_(first) {}

    const_iterator& operator++() {
      ++first_;
      return *this;
    }

    auto operator*() { return std::tuple(first_ - base_); }

    bool operator==(const const_iterator& b) const { return first_ == b.first_; }
    bool operator!=(const const_iterator& b) const { return first_ != b.first_; }
  };

  std::size_t size() const { return the_graph_.size(); }

  iterator begin() { return {the_graph_.begin(), the_graph_.begin()}; }
  iterator end() { return {the_graph_.begin(), the_graph_.end()}; }

  const_iterator begin() const { return {the_graph_.begin(), the_graph_.begin()}; }
  const_iterator end() const { return {the_graph_.begin(), the_graph_.end()}; }

private:
  Graph& the_graph_;
};
