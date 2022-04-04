---
layout: method
title: dag_range<Graph, Queue>
owner: __MISSING__
brief: __MISSING__
tags:
  - method
defined_in_file: dag_range.hpp
is_ctor: true
overloads:
  dag_range<Graph, Queue>(Graph &, nw::graph::dag_range::VOV, nw::graph::dag_range::VOV):
    arguments:
      - description: __OPTIONAL__
        name: graph
        type: Graph &
      - description: __OPTIONAL__
        name: pred_list
        type: nw::graph::dag_range::VOV
      - description: __OPTIONAL__
        name: successor_list
        type: nw::graph::dag_range::VOV
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: dag_range<Graph, Queue>(Graph & graph, nw::graph::dag_range::VOV pred_list, nw::graph::dag_range::VOV successor_list)
  dag_range<Graph, Queue>(const dag_range<Graph, Queue> &&):
    annotation:
      - delete
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: const dag_range<Graph, Queue> &&
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: dag_range<Graph, Queue>(const dag_range<Graph, Queue> &&)
  dag_range<Graph, Queue>(const dag_range<Graph, Queue> &):
    annotation:
      - delete
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: const dag_range<Graph, Queue> &
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: dag_range<Graph, Queue>(const dag_range<Graph, Queue> &)
---
