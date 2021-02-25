

// std::vector<forward_list>
template <typename... Attributes>
struct graph_traits<std::vector<std::forward_list<std::tuple<Attributes...>>>> {
  using tuple_type = std::tuple<Attributes...>;
  using inner_type = std::forward_list<tuple_type>;
  using outer_type = std::vector<inner_type>;

  using outer_iterator = typename outer_type::iterator;
  using inner_iterator = typename inner_type::iterator;

  using const_outer_iterator = typename outer_type::const_iterator;
  using const_inner_iterator = typename inner_type::const_iterator;

  using vertex_id_type    = typename std::tuple_element<0, tuple_type>::type;
  using vertex_size_type  = typename outer_type::size_type;
  using num_vertices_type = std::array<vertex_size_type, 1>;
};

template <typename... Attributes>
struct graph_traits<std::forward_list<std::tuple<Attributes...>>> {
  using tuple_type = std::tuple<Attributes...>;

  using vertex_id_type = typename std::tuple_element<0, tuple_type>::type;
};


// std::vector<vector>
template <typename... Attributes>
struct graph_traits<std::vector<std::vector<std::tuple<Attributes...>>>> {
  using tuple_type = std::tuple<Attributes...>;
  using inner_type = std::vector<tuple_type>;
  using outer_type = std::vector<inner_type>;

  using outer_iterator = typename outer_type::iterator;
  using inner_iterator = typename inner_type::iterator;

  using const_outer_iterator = typename outer_type::const_iterator;
  using const_inner_iterator = typename inner_type::const_iterator;

  using vertex_id_type    = typename std::tuple_element<0, tuple_type>::type;
  using vertex_size_type  = typename outer_type::size_type;
  using num_vertices_type = std::array<vertex_size_type, 1>;
};

template <typename... Attributes>
struct graph_traits<std::vector<std::tuple<Attributes...>>> {
  using tuple_type = std::tuple<Attributes...>;

  using vertex_id_type = typename std::tuple_element<0, tuple_type>::type;
};
