//
// This file is part of Standard Graph Library (SGL)
// (c) Pacific Northwest National Laboratory 2018
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0
// International License https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Kevin Deweese
//

#ifndef BETWEENNESS_CENTRALITY_HPP
#define BETWEENNESS_CENTRALITY_HPP

#include "util/util.hpp"
#include "util/types.hpp"
#include "util/AtomicBitVector.hpp"
#include "util/atomic.hpp"
#include "util/parallel_for.hpp"
#include "adaptors/worklist.hpp"

#include <algorithm>

#if defined(CL_SYCL_LANGUAGE_VERSION)
#include <dpstd/algorithm>
#include <dpstd/execution>
#include <dpstd/numeric>
#else
#include <execution>
#endif

#include <forward_list>
#include <future>
#include <iostream>
#include <list>
#include <mutex>
#include <queue>
#include <set>
#include <stack>
#include <thread>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include <tbb/concurrent_queue.h>
#include <tbb/concurrent_vector.h>
#include <tbb/parallel_for_each.h>

namespace nw {
namespace graph {

class Spinlock {
  std::atomic_flag flag = ATOMIC_FLAG_INIT;

public:
  void lock() {
    while (flag.test_and_set(std::memory_order_acquire)) {
    }
  }

  void unlock() { flag.clear(std::memory_order_release); }
};
//****************************************************************************
template <typename Graph, typename score_t = float, typename accum_t = size_t>
std::vector<score_t> betweenness_brandes(Graph& A, bool normalize = true) {
  size_t               n_vtx = A.size();
  std::vector<score_t> centrality(n_vtx, 0);
  auto                 G = A.begin();

  std::stack<vertex_id_t> S;
  std::queue<vertex_id_t> Q;
  std::vector<accum_t>    path_counts(n_vtx);
  std::vector<score_t>    d(n_vtx);

  for (auto outer = G; outer != A.end(); ++outer) {
    auto s = outer - G;

    path_counts.assign(n_vtx, 0);
    d.assign(n_vtx, -1);
    std::vector<std::list<size_t>> P(n_vtx);

    path_counts[s] = 1;
    d[s]           = 0;
    Q.push(s);

    while (!Q.empty()) {
      auto v = Q.front();
      Q.pop();
      S.push(v);
      for (auto inner = G[v].begin(); inner != G[v].end(); ++inner) {
        auto w = std::get<0>(*inner);
        if (d[w] < 0) {
          Q.push(w);
          d[w] = d[v] + 1;
        }
        if (d[w] == (d[v] + 1)) {
          path_counts[w] += path_counts[v];
          P[w].push_back(v);
        }
      }
    }

    std::vector<score_t> delta(n_vtx, 0);
    while (!S.empty()) {
      auto w = S.top();
      S.pop();
      for (auto it = P[w].begin(); it != P[w].end(); ++it) {
        delta[*it] += static_cast<score_t>(path_counts[*it]) / static_cast<score_t>(path_counts[w]) * (1 + delta[w]);
      }
      if (w != s) {
        centrality[w] += delta[w];
      }
    }
  }
  
  if (normalize) {
    score_t largest = *std::max_element(centrality.begin(), centrality.end());
    std::transform(centrality.begin(), centrality.end(), centrality.begin(), [&](auto &val) { return val /= largest; });
  }
  return centrality;
}

template <typename Graph, typename score_t = float, typename accum_t = size_t>
std::vector<score_t> approx_betweenness_brandes(Graph& A, std::vector<vertex_id_t>& sources) {
  size_t               n_vtx = A.size();
  std::vector<score_t> centrality(n_vtx, 0);
  auto                 G = A.begin();

  std::stack<vertex_id_t> S;
  std::queue<vertex_id_t> Q;
  std::vector<accum_t>    path_counts(n_vtx);
  std::vector<score_t>    d(n_vtx);

  for (auto& s : sources) {
    path_counts.assign(n_vtx, 0);
    d.assign(n_vtx, -1);
    std::vector<std::list<size_t>> P(n_vtx);

    path_counts[s] = 1;
    d[s]           = 0;
    Q.push(s);

    while (!Q.empty()) {
      auto v = Q.front();
      Q.pop();
      S.push(v);
      for (auto inner = G[v].begin(); inner != G[v].end(); ++inner) {
        auto w = std::get<0>(*inner);
        if (d[w] < 0) {
          Q.push(w);
          d[w] = d[v] + 1;
        }
        if (d[w] == (d[v] + 1)) {
          path_counts[w] += path_counts[v];
          P[w].push_back(v);
        }
      }
    }

    std::vector<score_t> delta(n_vtx, 0);
    while (!S.empty()) {
      auto w = S.top();
      S.pop();
      for (auto it = P[w].begin(); it != P[w].end(); ++it) {
        delta[*it] += static_cast<score_t>(path_counts[*it]) / static_cast<score_t>(path_counts[w]) * (1 + delta[w]);
      }
      if (w != s) {
        centrality[w] += delta[w];
      }
    }
  }

  score_t largest = *std::max_element(centrality.begin(), centrality.end());
  std::transform(centrality.begin(), centrality.end(), centrality.begin(), [&](auto& val) { return val /= largest; });
  return centrality;
}

template <typename Graph, typename score_t = float, typename accum_t = size_t>
std::vector<score_t> approx_betweenness_worklist_serial(Graph& A, std::vector<vertex_id_t>& sources) {
  size_t               n_vtx = A.size();
  std::vector<score_t> centrality(n_vtx, 0);
  auto                 G = A.begin();

  struct nodeinfo {
    size_t                                                      l;
    accum_t                                                     path_counts;
    std::unordered_set<vertex_id_t>                             preds;
    std::unordered_set<vertex_id_t>                             succs;
    std::unordered_map<vertex_id_t, std::pair<size_t, accum_t>> edge_l_s;
  };

  std::vector<size_t>   succs_ct(n_vtx);
  std::vector<score_t>  delta(n_vtx);
  std::vector<nodeinfo> vertices(n_vtx);

  auto g_cn = [](auto& u, auto& v, auto& udata, auto& vdata) {
    return udata->l >= vdata->l && udata->edge_l_s[v].first == udata->l && udata->l != std::numeric_limits<std::uint32_t>::max();
  };
  auto apply_cn = [](auto& u, auto& v, auto& udata, auto& vdata) {
    udata->succs.erase(v);
    udata->edge_l_s[v].first = std::numeric_limits<std::uint32_t>::max();
  };
  auto g_sp       = [](auto& u, auto& v, auto& udata, auto& vdata) { return vdata->l > udata->l + 1; };
  auto apply_spfu = [](auto& u, auto& v, auto& udata, auto& vdata) {
    vdata->l           = udata->l + 1;
    vdata->path_counts = udata->path_counts;
    vdata->preds.clear();
    vdata->succs.clear();
    udata->succs.emplace(v);
    vdata->preds.emplace(u);
    udata->edge_l_s[v].first  = udata->l;
    udata->edge_l_s[v].second = udata->path_counts;
  };
  auto g_fu = [](auto& u, auto& v, auto& udata, auto& vdata) {
    return vdata->l == udata->l + 1 && udata->edge_l_s[v].first != udata->l;
  };
  auto apply_fu = [](auto& u, auto& v, auto& udata, auto& vdata) {
    vdata->path_counts += udata->path_counts;
    udata->succs.emplace(v);
    vdata->preds.emplace(u);
    udata->edge_l_s[v].first  = udata->l;
    udata->edge_l_s[v].second = udata->path_counts;
  };
  auto g_us = [](auto& u, auto& v, auto& udata, auto& vdata) {
    return udata->l == udata->edge_l_s[v].first && vdata->l == udata->l + 1 && udata->edge_l_s[v].second != udata->path_counts;
  };
  auto apply_us = [](auto& u, auto& v, auto& udata, auto& vdata) {
    vdata->path_counts        = vdata->path_counts + udata->path_counts - udata->edge_l_s[v].second;
    udata->edge_l_s[v].second = udata->path_counts;
  };

  worklist_range<decltype(A), std::pair<vertex_id_t, vertex_id_t>> worklist(A);
  worklist_range<decltype(A), vertex_id_t>                         worklist_back(A);
  for (auto& s : sources) {

    for (auto it = vertices.begin(); it != vertices.end(); ++it) {
      it->l           = std::numeric_limits<std::uint32_t>::max();
      it->path_counts = 0;
      it->preds.clear();
      it->succs.clear();
      it->edge_l_s.clear();
    }

    vertices[s].l           = 0;
    vertices[s].path_counts = 1;
    std::fill(delta.begin(), delta.end(), 0);

    for (auto inner = G[s].begin(); inner != G[s].end(); ++inner) {
      worklist.push_back(std::pair(s, std::get<0>(*inner)));
    }

    auto work = worklist.begin();
    for (; work != worklist.end(); ++work) {
      // auto workitem = *(++work);
      auto workitem = *(work);
      auto u        = std::get<0>(workitem);
      auto v        = std::get<1>(workitem);

      nodeinfo* vdata = &vertices[v];
      nodeinfo* udata = &vertices[u];
      // lock u,v

      if (g_cn(u, v, udata, vdata)) {
        apply_cn(u, v, udata, vdata);
      } else if (g_sp(u, v, udata, vdata)) {
        std::vector<size_t> temp_preds(vdata->preds.size());
        size_t              i = 0;
        for (auto it = vdata->preds.begin(); it != vdata->preds.end(); ++it) {
          temp_preds[i] = *it;
          ++i;
        }
        apply_spfu(u, v, udata, vdata);
        // unlock u,v

        for (auto inner = G[v].begin(); inner != G[v].end(); ++inner) {
          worklist.push_back(std::pair(v, std::get<0>(*inner)));
        }
        // Should this be all incoming edges?
        for (auto iter = temp_preds.begin(); iter != temp_preds.end(); ++iter) {
          worklist.push_back(std::pair(*iter, v));
        }
      }    // else if (vdata->l == udata->l + 1 && udata->edge_l_s[v].first !=
      // udata->l) {
      // else if (vdata->l == udata->l + 1 && udata->edge_l[v] != udata->l) {
      else if (g_fu(u, v, udata, vdata)) {
        apply_fu(u, v, udata, vdata);
        // unlock u,v
        // should this include all neighbors of v?
        if (vdata->succs.begin() != vdata->succs.end()) {
          for (auto inner = G[v].begin(); inner != G[v].end(); ++inner) {
            worklist.push_back(std::pair(v, std::get<0>(*inner)));
          }
        }
        /*for (auto it = vertices[v].succs.begin(); it !=
        vertices[v].succs.end(); ++it) { worklist.push_back(std::pair(v, *it));
        }*/
      } else if (g_us(u, v, udata, vdata)) {
        apply_us(u, v, udata, vdata);
        // unlock u,v

        // should this only be all neighbors of v?
        /*for (auto inner = (A.begin()[v]).begin(); inner !=
        (A.begin()[v]).end(); ++inner) { worklist.push_back(std::pair(v,
        std::get<0>(*inner)));
        }*/
        for (auto it = vdata->succs.begin(); it != vdata->succs.end(); ++it) {
          worklist.push_back(std::pair(v, *it));
        }
      } else {
        // unlock u,v
      }
    }

    vertex_id_t id = 0;
    for (auto it = vertices.begin(); it != vertices.end(); ++it) {
      succs_ct[id] = it->succs.size();

      if (it->succs.empty()) {
        worklist_back.push_back(id);
      }
      ++id;
    }

    auto work2 = worklist_back.begin();
    for (; work2 != worklist_back.end(); ++work2) {
      auto      workitem = *(work2);
      auto      v        = workitem;
      nodeinfo* vdata    = &vertices[v];

      for (auto it = vdata->preds.begin(); it != vdata->preds.end(); ++it) {
        // lock u (*it)
        auto u = *it;
        delta[u] += static_cast<score_t>(vertices[u].path_counts) / static_cast<score_t>(vdata->path_counts) * (1 + delta[v]);
        succs_ct[u]--;
        if (succs_ct[u] == 0 && u != s) worklist_back.push_back(u);
      }
      centrality[v] += delta[v];
    }
  }

  score_t largest = *std::max_element(centrality.begin(), centrality.end());
  std::transform(centrality.begin(), centrality.end(), centrality.begin(), [&](auto& val) { return val /= largest; });
  return centrality;
}

template <typename Graph, typename score_t = float, typename accum_t = size_t>
std::vector<score_t> approx_betweenness_worklist(Graph& A, std::vector<vertex_id_t>& sources, size_t num_threads, size_t DELTA) {
  size_t               n_vtx = A.size();
  std::vector<score_t> centrality(n_vtx, 0);
  auto                 G = A.begin();
  // size_t             DELTA = 200;
  struct nodeinfo {
    size_t                                                      l;
    accum_t                                                     path_counts;
    std::unordered_set<vertex_id_t>                             preds;
    std::unordered_set<vertex_id_t>                             succs;
    std::unordered_map<vertex_id_t, std::pair<size_t, accum_t>> edge_l_s;
  };

  std::vector<size_t>     succs_ct(n_vtx);
  std::vector<score_t>    delta(n_vtx);
  std::vector<nodeinfo>   vertices(n_vtx);
  std::vector<Spinlock>   vector_spinlocks(n_vtx);
  std::vector<std::mutex> locks(n_vtx);
  Spinlock                worklock;
  auto                    g_cn = [](auto& u, auto& v, auto& udata, auto& vdata) {
    return udata->l >= vdata->l && udata->edge_l_s[v].first == udata->l && udata->l != std::numeric_limits<std::uint32_t>::max();
  };
  auto apply_cn = [](auto& u, auto& v, auto& udata, auto& vdata) {
    udata->succs.erase(v);
    udata->edge_l_s[v].first = std::numeric_limits<std::uint32_t>::max();
  };
  auto g_sp       = [](auto& u, auto& v, auto& udata, auto& vdata) { return vdata->l > udata->l + 1; };
  auto apply_spfu = [](auto& u, auto& v, auto& udata, auto& vdata) {
    vdata->l           = udata->l + 1;
    vdata->path_counts = udata->path_counts;
    vdata->preds.clear();
    vdata->succs.clear();
    udata->succs.emplace(v);
    vdata->preds.emplace(u);
    udata->edge_l_s[v].first  = udata->l;
    udata->edge_l_s[v].second = udata->path_counts;
  };
  auto g_fu = [](auto& u, auto& v, auto& udata, auto& vdata) {
    return vdata->l == udata->l + 1 && udata->edge_l_s[v].first != udata->l;
  };
  auto apply_fu = [](auto& u, auto& v, auto& udata, auto& vdata) {
    vdata->path_counts += udata->path_counts;
    udata->succs.emplace(v);
    vdata->preds.emplace(u);
    udata->edge_l_s[v].first  = udata->l;
    udata->edge_l_s[v].second = udata->path_counts;
  };
  auto g_us = [](auto& u, auto& v, auto& udata, auto& vdata) {
    return udata->l == udata->edge_l_s[v].first && vdata->l == udata->l + 1 && udata->edge_l_s[v].second != udata->path_counts;
  };
  auto apply_us = [](auto& u, auto& v, auto& udata, auto& vdata) {
    vdata->path_counts        = vdata->path_counts + udata->path_counts - udata->edge_l_s[v].second;
    udata->edge_l_s[v].second = udata->path_counts;
  };

  for (auto& s : sources) {
    for (auto it = vertices.begin(); it != vertices.end(); ++it) {
      it->l           = std::numeric_limits<std::uint32_t>::max();
      it->path_counts = 0;
      it->preds.clear();
      it->succs.clear();
      it->edge_l_s.clear();
    }

    vertices[s].l           = 0;
    vertices[s].path_counts = 1;
    std::fill(delta.begin(), delta.end(), 0);
    tbb::concurrent_queue<std::pair<vertex_id_t, vertex_id_t>> myqueue;
    // tbbworklist_range<decltype(A), std::pair<std::pair<vertex_id_t,
    // vertex_id_t>, size_t>, DELTAQ> worklist(A);
    tbbworklist_range2<decltype(A), std::pair<vertex_id_t, vertex_id_t>> worklist(A);
    auto                                                                 first = A.begin();
    for (auto inner = (first[s]).begin(); inner != (first[s]).end(); ++inner) {
      // worklist.push_back(std::pair(s, std::get<0>(*inner)));
      worklist.push_back(std::pair(s, std::get<0>(*inner)), 0);
      // myqueue.push(std::pair(s, std::get<0>(*inner)));
    }
    std::pair<vertex_id_t, vertex_id_t> dummy1(0, 0);
    worklist.set_dummy(dummy1);

    std::vector<std::future<void>> futures(num_threads);
    for (size_t thread = 0; thread < num_threads; ++thread) {
      futures[thread] = std::async(
          std::launch::async,
          [&](size_t thread) {
            thread_local auto work = worklist.begin();
            for (; work != worklist.end(); ++work) {
              if (*work != dummy1) {

                // while (myqueue.unsafe_size() > 0) {
                /*size_t buffer_size=50;
                                       std::vector<std::pair<vertex_id_t,
                   vertex_id_t>> buffer(buffer_size); for(size_t j = 0; j <
                   buffer_size; ++j) { buffer[j] = *(++work);
                                       }
                                       for(size_t j = 0; j < buffer_size; ++j) {
                                         auto workitem = buffer[j];*/
                // auto      workitembuffer = *(++work);
                // std::cout << "anything" << std::endl;
                // for(auto workitem = (*work).begin(); workitem !=
                // (*work).end(); ++workitem) {
                // std::for_each(std::execution::par_unseq,(*work).begin(),
                // (*work).end(), [&](auto &workitem) {

                // for (auto workitem = (*work).begin(); workitem !=
                // (*work).end(); ++workitem) { auto workitem = *y;
                // std::pair<vertex_id_t, vertex_id_t> workitem;
                // if (myqueue.try_pop(workitem)) {
                // if(*workitem != dummy1) {
                auto u = std::get<0>(*work);
                auto v = std::get<1>(*work);
                if (u < v) {
                  vector_spinlocks[u].lock();
                  vector_spinlocks[v].lock();
                } else {
                  vector_spinlocks[v].lock();
                  vector_spinlocks[u].lock();
                }

                // std::cout << u << " " << v << std::endl;
                nodeinfo* udata = &vertices[u];
                nodeinfo* vdata = &vertices[v];
                // lock u,v
                if (g_cn(u, v, udata, vdata)) {
                  apply_cn(u, v, udata, vdata);
                  vector_spinlocks[v].unlock();
                  vector_spinlocks[u].unlock();
                } else if (g_sp(u, v, udata, vdata)) {
                  std::vector<size_t> temp_preds(vdata->preds.size());
                  size_t              i = 0;
                  for (auto it = vdata->preds.begin(); it != vdata->preds.end(); ++it) {
                    temp_preds[i] = *it;
                    ++i;
                  }
                  apply_spfu(u, v, udata, vdata);
                  // unlock u,v
                  vector_spinlocks[u].unlock();

                  for (auto inner = (A.begin()[v]).begin(); inner != (A.begin()[v]).end(); ++inner) {
                    size_t priority = (vdata->l + 1) / DELTA;
                    // worklist.push_back(std::pair(v, std::get<0>(*inner)));
                    // myqueue.push(std::pair(v, std::get<0>(*inner)));
                    worklist.push_back(std::pair(v, std::get<0>(*inner)), priority);
                  }

                  // Should this be all incoming edges?
                  for (auto iter = temp_preds.begin(); iter != temp_preds.end(); ++iter) {
                    size_t priority = (vertices[*iter].l + 1) / DELTA;
                    // worklist.push_back(std::pair(*iter, v));
                    // myqueue.push(std::pair(*iter, v));
                    worklist.push_back(std::pair(*iter, v), priority);
                  }
                  vector_spinlocks[v].unlock();
                } else if (g_fu(u, v, udata, vdata)) {
                  apply_fu(u, v, udata, vdata);
                  // unlock u,v
                  vector_spinlocks[u].unlock();

                  // should this only be succ of v?
                  if (vdata->succs.begin() != vdata->succs.end()) {
                    for (auto inner = (A.begin()[v]).begin(); inner != (A.begin()[v]).end(); ++inner) {
                      size_t priority = (vdata->l + 1) / DELTA;
                      // worklist.push_back(std::pair(v, std::get<0>(*inner)));
                      // myqueue.push(std::pair(v, std::get<0>(*inner)));
                      worklist.push_back(std::pair(v, std::get<0>(*inner)), priority);
                    }
                  }
                  vector_spinlocks[v].unlock();
                  /*for (auto it = vdata->succs.begin(); it !=
                     vdata->succs.end(); ++it) { worklist.push_back(std::pair(v,
                     *it));
                       }*/
                } else if (g_us(u, v, udata, vdata)) {
                  apply_us(u, v, udata, vdata);
                  // unlock u,v
                  vector_spinlocks[u].unlock();

                  // should this only be succ of v?
                  /*for (auto inner = (A.begin()[v]).begin(); inner !=
                     (A.begin()[v]).end(); ++inner) {
                         worklist.push_back(std::pair(v, std::get<0>(*inner)));
                       }*/

                  for (auto it = vdata->succs.begin(); it != vdata->succs.end(); ++it) {
                    size_t priority = (vdata->l + 1) / DELTA;
                    worklist.push_back(std::pair(v, *it), priority);
                    // worklist.push_back(std::pair(v, *it));
                    // myqueue.push(std::pair(v, *it));
                  }
                  vector_spinlocks[v].unlock();
                } else {
                  // unlock u,v
                  vector_spinlocks[v].unlock();
                  vector_spinlocks[u].unlock();
                }
              }
            }
            return;
          },
          thread);
    }
    for (size_t i = 0; i < num_threads; ++i) {
      futures[i].get();
    }
    // std::cout << "here" << std::endl;
    for (auto work = worklist.begin(); work != worklist.end(); ++work) {
      // auto      workitembuffer = *(++work);
      // for (auto workitem = (*work).begin(); workitem != (*work).end();
      // ++workitem) {
      if (*work != dummy1) {
        auto u = std::get<0>(*work);
        auto v = std::get<1>(*work);
        std::cout << u << " " << v << std::endl;
        nodeinfo* vdata = &vertices[v];
        nodeinfo* udata = &vertices[u];

        if (g_cn(u, v, udata, vdata)) {
          apply_cn(u, v, udata, vdata);
        } else if (g_sp(u, v, udata, vdata)) {
          std::vector<size_t> temp_preds(vdata->preds.size());
          size_t              i = 0;
          for (auto it = vdata->preds.begin(); it != vdata->preds.end(); ++it) {
            temp_preds[i] = *it;
            ++i;
          }
          apply_spfu(u, v, udata, vdata);
          // unlock u,v

          for (auto inner = (A.begin()[v]).begin(); inner != (A.begin()[v]).end(); ++inner) {
            size_t priority = (vdata->l + 1) / DELTA;
            // worklist.push_back(std::pair(v, std::get<0>(*inner)));
            worklist.push_back(std::pair(v, std::get<0>(*inner)), priority);
          }
          // Should this be all incoming edges?
          for (auto iter = temp_preds.begin(); iter != temp_preds.end(); ++iter) {
            size_t priority = (vertices[*iter].l + 1) / DELTA;
            // worklist.push_back(std::pair(*iter, v));
            worklist.push_back(std::pair(*iter, v), priority);
          }
        } else if (g_fu(u, v, udata, vdata)) {
          apply_fu(u, v, udata, vdata);
          // unlock u,v
          // should this include all neighbors of v?
          if (vdata->succs.begin() != vdata->succs.end()) {
            for (auto inner = (A.begin()[v]).begin(); inner != (A.begin()[v]).end(); ++inner) {
              size_t priority = (vdata->l + 1) / DELTA;
              worklist.push_back(std::pair(v, std::get<0>(*inner)), priority);
            }
          }
        } else if (g_us(u, v, udata, vdata)) {
          apply_us(u, v, udata, vdata);
          for (auto it = vdata->succs.begin(); it != vdata->succs.end(); ++it) {
            size_t priority = (vdata->l + 1) / DELTA;
            // worklist.push_back(std::pair(v, *it));
            worklist.push_back(std::pair(v, *it), priority);
          }
        }
      }
    }

    std::cout << "back" << std::endl;
    tbbworklist_range<decltype(A), vertex_id_t> worklist_back(A);

    std::vector<size_t> succs_ct(n_vtx);
    // std::cout << "back" << std::endl;
    for (vertex_id_t v = 0; v < A.max() + 1; ++v) {
      succs_ct[v] = vertices[v].succs.size();
      if (vertices[v].succs.empty()) {
        worklist_back.push_back(v);
      }
    }

    worklist_back.set_dummy(std::numeric_limits<std::uint32_t>::max());
    std::vector<std::future<void>> futures2(num_threads);
    // std::cout << "num_threads " << num_threads << std::endl;
    for (size_t thread = 0; thread < num_threads; ++thread) {
      futures2[thread] = std::async(
          std::launch::async,
          [&](size_t thread) {
            // std::cout << "starting on " << thread << std::endl;
            thread_local auto work2 = worklist_back.begin();
            for (; work2 != worklist_back.end(); ++work2) {
              // auto workitembuffer = *(++work2);
              // for (auto y : workitembuffer) {
              // auto workitem = y;
              if (*work2 != std::numeric_limits<std::uint32_t>::max()) {
                auto v = *work2;
                // std::cout << "thread " << thread << " taking " << v <<
                // std::endl; vector_spinlocks[v].lock(); locks[v].lock();
                // std::cout << "thread " << thread << " got " << v <<
                // std::endl;
                for (auto it = vertices[v].preds.begin(); it != vertices[v].preds.end(); ++it) {
                  auto u = *it;
                  // std::cout << "u " << u << std::endl;
                  vector_spinlocks[u].lock();
                  // locks[u].lock();
                  delta[u] = delta[u] + static_cast<score_t>(vertices[u].path_counts) /
                                            static_cast<score_t>(vertices[v].path_counts) * (1 + delta[v]);
                  // succs[u].erase(v);
                  succs_ct[u]--;
                  // if (succs[u].empty() && u != s) worklist_back.push_back(u);
                  if (succs_ct[u] == 0 && u != s) {
                    worklist_back.push_back(u);
                  }
                  vector_spinlocks[u].unlock();
                  // locks[u].unlock();
                }
                centrality[v] += delta[v];
                // vector_spinlocks[v].unlock();
                // locks[v].unlock();
              }
            }

            // std::cout << "thread " << thread << " done" << std::endl;

            return;
          },
          thread);
    }
    for (size_t i = 0; i < num_threads; ++i) {
      futures2[i].get();
    }
  }
  return centrality;
}

template <typename Graph, typename score_t = float, typename accum_t = size_t>
std::vector<score_t> approx_betweenness_worklist_noabstraction(Graph& A, std::vector<vertex_id_t>& sources, size_t num_threads,
                                                               size_t par_thresh, size_t DELTA = 1) {
  size_t               n_vtx = A.size();
  std::vector<score_t> centrality(n_vtx, 0.0);
  auto                 G               = A.begin();
  bool                 levelsets       = true;
  size_t               back_buffersize = 100;
  bool                 asyncv1         = true;

  struct nodeinfo {
    size_t                                                      l;
    accum_t                                                     path_counts;
    std::forward_list<vertex_id_t>                              preds;
    std::forward_list<vertex_id_t>                              succs;
    std::unordered_map<vertex_id_t, std::pair<size_t, accum_t>> edge_l_s;
  };

  auto g_cn = [](auto& u, auto& v, auto& udata, auto& vdata) {
    return udata->l >= vdata->l && udata->edge_l_s[v].first == udata->l && udata->l != std::numeric_limits<std::uint32_t>::max();
  };
  auto apply_cn = [](auto& u, auto& v, auto& udata, auto& vdata) {
    udata->succs.remove(v);
    udata->edge_l_s[v].first = std::numeric_limits<std::uint32_t>::max();
  };
  auto g_sp       = [](auto& u, auto& v, auto& udata, auto& vdata) { return vdata->l > udata->l + 1; };
  auto apply_spfu = [](auto& u, auto& v, auto& udata, auto& vdata) {
    vdata->l           = udata->l + 1;
    vdata->path_counts = udata->path_counts;
    vdata->preds.clear();
    vdata->succs.clear();
    udata->succs.emplace_front(v);
    udata->edge_l_s[v].first  = udata->l;
    udata->edge_l_s[v].second = udata->path_counts;
    vdata->preds.emplace_front(u);
  };
  auto g_fu = [](auto& u, auto& v, auto& udata, auto& vdata) {
    return vdata->l == udata->l + 1 && udata->edge_l_s[v].first != udata->l;
  };
  auto apply_fu = [](auto& u, auto& v, auto& udata, auto& vdata) {
    vdata->path_counts += udata->path_counts;
    udata->succs.emplace_front(v);
    // vdata->preds.push_back(u);
    udata->edge_l_s[v].first  = udata->l;
    udata->edge_l_s[v].second = udata->path_counts;
    // udata->succs.emplace_front(std::tuple(v, udata->l, udata->path_counts));
    vdata->preds.emplace_front(u);
  };
  auto g_us = [](auto& u, auto& v, auto& udata, auto& vdata) {
    return udata->l == udata->edge_l_s[v].first && vdata->l == udata->l + 1 && udata->edge_l_s[v].second != udata->path_counts;
    // return udata->l == std::get<1>(*it) && vdata->l == udata->l + 1 &&
    // std::get<2>(*it) != udata->path_counts;
  };
  auto apply_us = [](auto& u, auto& v, auto& udata, auto& vdata) {
    vdata->path_counts        = vdata->path_counts + udata->path_counts - udata->edge_l_s[v].second;
    udata->edge_l_s[v].second = udata->path_counts;
  };

  std::vector<size_t>                      succs_ct(n_vtx);
  std::vector<score_t>                     delta(n_vtx);
  std::vector<Spinlock>                    vector_spinlocks(n_vtx);
  std::vector<nodeinfo>                    vertices(n_vtx);
  std::atomic<size_t>                      num_buckets = 10;
  Spinlock                                 b_lock;
  bool                                     parallel;
  std::function<void(vertex_id_t, size_t)> innerfunc = [&](vertex_id_t s, size_t num_threads) {
    std::function<void(size_t, size_t)> reset([&](size_t begin, size_t end) {
      for (size_t v = begin; v < end; ++v) {
        vertices[v].l           = std::numeric_limits<std::uint32_t>::max();
        vertices[v].path_counts = 0;
        vertices[v].preds.clear();
        vertices[v].succs.clear();
        delta[v]    = 0;
        succs_ct[v] = 0;
        vector_spinlocks[v].unlock();
      }
    });

    std::vector<std::thread> vecOfThreads;

    if (num_threads != 1) {
      for (size_t thread = 0; thread < num_threads; ++thread) {
        vecOfThreads.push_back(std::thread(reset, n_vtx / num_threads * thread,
                                           (thread == num_threads - 1) ? n_vtx : n_vtx / num_threads * (thread + 1)));
      }
      for (std::thread& th : vecOfThreads) {
        if (th.joinable()) th.join();
      }
    } else {
      reset(0, n_vtx);
    }

    vertices[s].l           = 0;
    vertices[s].path_counts = 1;

    if (asyncv1) {
      tbb::concurrent_vector<tbb::concurrent_vector<std::pair<vertex_id_t, vertex_id_t>>> buckets(num_buckets);
      tbb::concurrent_vector<std::pair<vertex_id_t, vertex_id_t>>                         currentlevel;
      for (auto it = G[s].begin(); it != G[s].end(); ++it) {
        buckets[0].push_back(std::pair(s, std::get<0>(*it)));
      }

      auto async1 = [&](auto& workpair) {
        auto u = std::get<0>(workpair);
        auto v = std::get<1>(workpair);
        // std::cout << u << " " << v << std::endl;
        if (parallel) {
          if (u < v) {
            vector_spinlocks[u].lock();
            vector_spinlocks[v].lock();
          } else {
            vector_spinlocks[v].lock();
            vector_spinlocks[u].lock();
          }
        }

        nodeinfo* udata = &vertices[u];
        nodeinfo* vdata = &vertices[v];
        // lock u,v
        if (g_cn(u, v, udata, vdata)) {
          apply_cn(u, v, udata, vdata);
          if (parallel) {
            vector_spinlocks[v].unlock();
            vector_spinlocks[u].unlock();
          }
        } else if (g_sp(u, v, udata, vdata)) {
          auto temp_preds = vdata->preds;

          apply_spfu(u, v, udata, vdata);
          // unlock u,v
          if (parallel) {
            vector_spinlocks[u].unlock();
            vector_spinlocks[v].unlock();
          }
          for (auto inner = (A.begin()[v]).begin(); inner != (A.begin()[v]).end(); ++inner) {
            size_t priority = (vdata->l + 1) / DELTA;
            // if (priority > maxbucket) {
            if (priority + 1 > num_buckets) {
              if (parallel) b_lock.lock();
              buckets.grow_to_at_least(priority + 1);
              num_buckets = priority + 1;
              if (parallel) b_lock.unlock();
            }
            // maxbucket = priority;
            //}
            buckets[priority].push_back(std::pair(v, std::get<0>(*inner)));
          }

          // Should this be all incoming edges?
          for (auto iter = temp_preds.begin(); iter != temp_preds.end(); ++iter) {
            size_t priority = (vdata->l + 1) / DELTA;
            // if (priority > maxbucket) {
            if (priority + 1 > num_buckets) {
              if (parallel) b_lock.lock();
              buckets.grow_to_at_least(priority + 1);
              num_buckets = priority + 1;
              if (parallel) b_lock.unlock();
            }
            // maxbucket = priority;
            //}
            buckets[priority].push_back(std::pair(*iter, v));
          }

        } else if (g_fu(u, v, udata, vdata)) {
          apply_fu(u, v, udata, vdata);
          // unlock u,v
          if (parallel) {
            vector_spinlocks[u].unlock();
            vector_spinlocks[v].unlock();
          }
          // should this only be succ of v?
          if (vdata->succs.begin() != vdata->succs.end()) {
            for (auto inner = (A.begin()[v]).begin(); inner != (A.begin()[v]).end(); ++inner) {
              size_t priority = (vdata->l + 1) / DELTA;
              // if (priority > maxbucket) {
              if (priority + 1 > num_buckets) {
                if (parallel) b_lock.lock();
                buckets.grow_to_at_least(priority + 1);
                num_buckets = priority + 1;
                if (parallel) b_lock.unlock();
              }
              // maxbucket = priority;
              //}
              buckets[priority].push_back(std::pair(v, std::get<0>(*inner)));
            }
          }
        } else if (g_us(u, v, udata, vdata)) {
          apply_us(u, v, udata, vdata);
          // unlock u,v
          if (parallel) {
            vector_spinlocks[u].unlock();
            vector_spinlocks[v].unlock();
          }
          for (auto it = vdata->succs.begin(); it != vdata->succs.end(); ++it) {
            size_t priority = (vdata->l + 1) / DELTA;
            // if (priority > maxbucket) {
            if (priority + 1 > num_buckets) {
              if (parallel) b_lock.lock();
              buckets.grow_to_at_least(priority + 1);
              num_buckets = priority + 1;
              if (parallel) b_lock.unlock();
            }
            // maxbucket = priority;
            //}
            buckets[priority].push_back(std::pair(v, *it));
          }

        } else if (parallel) {
          // unlock u,v
          vector_spinlocks[v].unlock();
          vector_spinlocks[u].unlock();
        }
        return;
      };

      size_t top_level = 0;
      while (top_level < buckets.size()) {
        currentlevel.resize(0);
        std::swap(currentlevel, buckets[top_level]);
        if (num_threads != 1 && currentlevel.size() > par_thresh) {
          parallel = true;
          std::for_each(std::execution::par, currentlevel.begin(), currentlevel.end(), async1);
        } else {
          parallel = false;
          std::for_each(currentlevel.begin(), currentlevel.end(), async1);
        }
        while (buckets[top_level].empty() && top_level < buckets.size()) {
          top_level++;
        }
      }

    } else {
      tbb::concurrent_vector<tbb::concurrent_vector<vertex_id_t>> buckets(num_buckets);
      tbb::concurrent_vector<vertex_id_t>                         currentlevel;
      buckets[0].push_back(s);
      auto async2 = [&](auto& u) {
        for (auto inner = G[u].begin(); inner != G[u].end(); ++inner) {
          auto v = std::get<0>(*inner);
          if (u == v) {
            continue;
          }

          if (parallel) {
            if (u < v) {
              vector_spinlocks[u].lock();
              vector_spinlocks[v].lock();
            } else {
              vector_spinlocks[v].lock();
              vector_spinlocks[u].lock();
            }
          }

          nodeinfo* udata = &vertices[u];
          nodeinfo* vdata = &vertices[v];
          // lock u,v
          if (g_sp(u, v, udata, vdata)) {
            auto temp_preds = vdata->preds;
            apply_spfu(u, v, udata, vdata);
            // unlock u,v
            if (parallel) {
              vector_spinlocks[u].unlock();
              vector_spinlocks[v].unlock();
            }
            size_t priority = (vdata->l + 1) / DELTA;
            // if (priority > maxbucket) {
            if (priority + 1 > num_buckets) {
              if (parallel) b_lock.lock();
              buckets.grow_to_at_least(priority + 1);
              num_buckets = priority + 1;
              if (parallel) b_lock.unlock();
            }
            // maxbucket = priority;
            //}
            buckets[priority].push_back(v);

            for (auto inner2 = temp_preds.begin(); inner2 != temp_preds.end(); ++inner2) {
              auto w = *inner2;

              if (parallel) {
                if (v == w) continue;
                if (v < w) {
                  vector_spinlocks[v].lock();
                  vector_spinlocks[w].lock();
                } else {
                  vector_spinlocks[w].lock();
                  vector_spinlocks[v].lock();
                }
              }

              nodeinfo* wdata = &vertices[w];

              if (g_cn(w, v, wdata, vdata)) {
                apply_cn(w, v, wdata, vdata);
              }
              if (parallel) {
                vector_spinlocks[v].unlock();
                vector_spinlocks[w].unlock();
              }
            }
          } else if (g_fu(u, v, udata, vdata)) {
            apply_fu(u, v, udata, vdata);
            // unlock u,v
            if (parallel) {
              vector_spinlocks[u].unlock();
              vector_spinlocks[v].unlock();
            }
            // should this only be succ of v?
            if (vdata->succs.begin() != vdata->succs.end()) {
              size_t priority = (vdata->l + 1) / DELTA;
              // if (priority > maxbucket) {
              if (priority + 1 > num_buckets) {
                if (parallel) b_lock.lock();
                buckets.grow_to_at_least(priority + 1);
                num_buckets = priority + 1;
                if (parallel) b_lock.unlock();
              }
              // maxbucket = priority;
              //}
              buckets[priority].push_back(v);
            }
          } else if (g_us(u, v, udata, vdata)) {
            apply_us(u, v, udata, vdata);
            // unlock u,v
            if (parallel) {
              vector_spinlocks[u].unlock();
              vector_spinlocks[v].unlock();
            }
            if (vdata->succs.begin() != vdata->succs.end()) {
              size_t priority = (vdata->l + 1) / DELTA;
              // if (priority > maxbucket) {
              if (priority + 1 > num_buckets) {
                if (parallel) b_lock.lock();
                buckets.grow_to_at_least(priority + 1);
                num_buckets = priority + 1;
                if (parallel) b_lock.unlock();
              }
              // maxbucket = priority;
              //}
              buckets[priority].push_back(v);
            }
          } else if (parallel) {
            // unlock u,v
            vector_spinlocks[v].unlock();
            vector_spinlocks[u].unlock();
          }
        }
        return;
      };

      size_t top_level = 0;
      while (top_level < buckets.size()) {
        currentlevel.resize(0);
        std::swap(currentlevel, buckets[top_level]);
        if (num_threads != 1 && currentlevel.size() > par_thresh) {
          parallel = true;
          std::for_each(std::execution::par, currentlevel.begin(), currentlevel.end(), async2);
        } else {
          parallel = false;
          std::for_each(currentlevel.begin(), currentlevel.end(), async2);
        }
        while (buckets[top_level].empty() && top_level < buckets.size()) {
          top_level++;
        }
      }
    }
    std::atomic<size_t>                workest = 0;
    tbb::concurrent_queue<vertex_id_t> myqueue2;

    if (num_threads == 1) {
      for (vertex_id_t v = 0; v < n_vtx; ++v) {
        for (auto it = vertices[v].succs.begin(); it != vertices[v].succs.end(); ++it) {
          succs_ct[v]++;
        }
        vertices[v].succs.clear();
        if (succs_ct[v] == 0) {
          myqueue2.push(v);
          workest++;
        }
      }
    } else {
      std::function<void(size_t, size_t)> populate_queue = [&](size_t begin, size_t end) {
        for (vertex_id_t v = begin; v < end; ++v) {
          succs_ct[v] = 0;
          for (auto it = vertices[v].succs.begin(); it != vertices[v].succs.end(); ++it) {
            succs_ct[v]++;
          }
          if (succs_ct[v] == 0) {
            myqueue2.push(v);
          }
        }
      };
      vecOfThreads.clear();
      for (size_t thread = 0; thread < num_threads; ++thread) {
        vecOfThreads.push_back(std::thread(populate_queue, n_vtx / num_threads * thread,
                                           (thread == num_threads - 1) ? n_vtx : n_vtx / num_threads * (thread + 1)));
      }
      for (std::thread& th : vecOfThreads) {
        if (th.joinable()) th.join();
      }
    }

    std::function<void(bool, bool)> backprocess = [&](bool parallel, bool final) {
      std::queue<vertex_id_t> buffer;
      vertex_id_t             workitem;
      while (buffer.size() > 0 || (parallel) || (!parallel && (workest < par_thresh || num_threads == 1)) || final) {
        bool found = false;
        if (buffer.size() > 0) {
          found    = true;
          workitem = buffer.front();
          buffer.pop();
        } else {
          found          = myqueue2.try_pop(workitem);
          size_t counter = 0;
          while (!found && parallel && counter < 10) {
            found = myqueue2.try_pop(workitem);
            counter++;
          }
        }
        if (!found) break;

        auto    v    = workitem;
        score_t temp = (1 + delta[v]) / static_cast<score_t>(vertices[v].path_counts);
        for (auto it = vertices[v].preds.begin(); it != vertices[v].preds.end(); ++it) {
          auto u = *it;
          if (parallel) vector_spinlocks[u].lock();

          delta[u] = delta[u] + temp;
          succs_ct[u]--;

          if (succs_ct[u] == 0 && u != s) {
            delta[u] *= static_cast<score_t>(vertices[u].path_counts);
            if (buffer.size() < back_buffersize) {
              buffer.push(u);
            } else {
              myqueue2.push(u);
            }
          } else if (parallel)
            vector_spinlocks[u].unlock();
        }
        centrality[v] += delta[v];
      }
    };

    if (num_threads != 1) {
      vecOfThreads.clear();
      for (size_t thread = 0; thread < num_threads; ++thread) {
        vecOfThreads.emplace_back(std::thread(backprocess, true, false));
      }
      for (std::thread& th : vecOfThreads) {
        if (th.joinable()) th.join();
      }
    }
    backprocess(false, true);

    return;
  };

  for (auto y : sources) {
    innerfunc(y, num_threads);
  }

  if (num_threads == 1) {
    score_t largest = 0;
    for (vertex_id_t v = 0; v < n_vtx; ++v) {
      largest = std::max(centrality[v], largest);
    }
    for (vertex_id_t v = 0; v < n_vtx; ++v) {
      centrality[v] = centrality[v] / largest;
    }
  } else {
    std::vector<std::future<score_t>> futures(num_threads);
    for (size_t thread = 0; thread < num_threads; ++thread) {
      size_t begin    = (n_vtx / num_threads) * thread;
      size_t end      = (thread == num_threads - 1) ? n_vtx : n_vtx / num_threads * (thread + 1);
      futures[thread] = std::async(
          std::launch::async,
          [&](size_t thread, size_t begin, size_t end) {
            score_t temp_largest = 0;
            for (vertex_id_t v = begin; v < end; ++v) {
              temp_largest = std::max(centrality[v], temp_largest);
            }
            return temp_largest;
          },
          thread, begin, end);
    }
    score_t largest = 0;
    for (size_t i = 0; i < num_threads; ++i) {
      largest = std::max(futures[i].get(), largest);
    }
    std::vector<std::future<void>> futures2(num_threads);
    for (size_t thread = 0; thread < num_threads; ++thread) {
      size_t begin     = n_vtx / num_threads * thread;
      size_t end       = (thread == num_threads - 1) ? n_vtx : n_vtx / num_threads * (thread + 1);
      futures2[thread] = std::async(
          std::launch::async,
          [&](size_t thread, size_t begin, size_t end) {
            for (vertex_id_t v = begin; v < end; ++v) {
              centrality[v] = centrality[v] / largest;
            }
            return;
          },
          thread, begin, end);
    }
    for (size_t i = 0; i < num_threads; ++i) {
      futures2[i].get();
    }
  }
  return centrality;
}

#if 0
template <typename Graph, typename Vector1, typename BitMap1, typename Queue1, typename Queue2>
void PBFS(const Graph &graph, vertex_id_t root, Vector1 &path_counts,
    BitMap1 &succ, Queue1 &depth_index, Queue2 &q1, Queue2 &q2) {

  std::vector<vertex_id_t> level(graph.num_nodes(), std::numeric_limits<vertex_id_t>::max());

  q1[0].push_back(root);
  level[root] = 0;
  path_counts[root] = 1;

  queue.push_back(root);
  depth_index.push_back(queue.begin());
  queue.slide_window();

  auto g_out_start = graph.out_neigh(0).begin();

  {
    vertex_id_t depth = 0;
    QueueBuffer<vertex_id_t> lqueue(queue);

    while (!queue.empty()) {

      depth_index.push_back(queue.begin());
      ++depth;

      for (auto q_iter = queue.begin(); q_iter < queue.end(); q_iter++) {
        auto u = *q_iter;
        vertex_id_t neg_one = -1;
        for (vertex_id_t &v : graph.out_neigh(u)) {
          if ((level[v] == -1) &&
              level[v].compare_exchange_strong, neg_one, depth)) {
            lqueue.push_back(v);
          }
          if (level[v] == depth) {
            succ.store(&v - g_out_start);
            path_counts[v].fetch_add(path_counts[u]);
          }
        }
      }
      lqueue.flush();
      queue.slide_window();
    }
  }

  depth_index.push_back(queue.begin());
}


std::vector<score_t> Brandes(const Graph &g, const std::vector<vertex_id_t> sources) {

  std::vector<score_t> scores(g.num_nodes(), 0);
  std::vector<score_t> path_counts(g.num_nodes());
  std::vector<std::atomic<bool>> succ(g.num_edges_directed());

  std::vector<SlidingQueue<vertex_id_t>::iterator> depth_index;
  SlidingQueue<vertex_id_t> queue(g.num_nodes());


  const vertex_id_t* g_out_start = g.out_neigh(0).begin();


  for (vertex_id_t iter=0; iter < num_iters; iter++) {
    vertex_id_t source = sp.PickNext();
    cout << "source: " << source << endl;
    t.Start();
    path_counts.fill(0);
    depth_index.resize(0);
    queue.reset();
    succ.reset();

    PBFS(g, source, path_counts, succ, depth_index, queue);

    std::vector<score_t> deltas(g.num_nodes(), 0);

    for (int d = depth_index.size()-2; d >= 0; d--) {

      for (auto it = depth_index[d]; it < depth_index[d+1]; it++) {
        vertex_id_t u = *it;
        score_t delta_u = 0;
        for (vertex_id_t &v : g.out_neigh(u)) {
          if (succ.get_bit(&v - g_out_start)) {
            delta_u += path_counts[u] / path_counts[v] * (1 + deltas[v]);
          }
        }
        deltas[u] = delta_u;
        scores[u] += delta_u;
      }
    }
  }

  score_t biggest_score = 0;

  for (vertex_id_t n=0; n < g.num_nodes(); n++)
    biggest_score = max(biggest_score, scores[n]);

  for (vertex_id_t n=0; n < g.num_nodes(); n++)
    scores[n] = scores[n] / biggest_score;

  return scores;
}
#endif

template <class score_t, class accum_t, class Graph>
bool BCVerifier(Graph&& g, std::vector<vertex_id_t> &trial_sources, std::vector<score_t> &scores_to_test) {
  std::vector<score_t> scores(g.max() + 1, 0);
  for(auto& source : trial_sources) {
    std::vector<int> depths(g.max() + 1, -1);
    depths[source] = 0;
    std::vector<accum_t> path_counts(g.max() + 1, 0);
    path_counts[source] = 1;
    std::vector<vertex_id_t> to_visit;
    to_visit.reserve(g.max() + 1);
    to_visit.push_back(source);
    auto out_neigh = g.begin();
    for(auto it = to_visit.begin(); it != to_visit.end(); it++) {
      vertex_id_t u = *it;
      for(auto edge : out_neigh[u]) {
    vertex_id_t v = std::get<0>(edge);
    if(depths[v] == -1) {
      depths[v] = depths[u] + 1;
      to_visit.push_back(v);
    }
    if(depths[v] == depths[u] + 1) {
      path_counts[v] += path_counts[u];
    }
      }
    }

    std::vector<std::vector<vertex_id_t>> verts_at_depth;
    for(size_t i = 0; i < g.max() + 1; ++i) {
      if(depths[i] != -1) {
    if(depths[i] >= static_cast<int>(verts_at_depth.size())) {
      verts_at_depth.resize(depths[i] + 1);
    }
    verts_at_depth[depths[i]].push_back(i);
      }
    }

    std::vector<score_t> deltas(g.max() + 1, 0);
    for(int depth=verts_at_depth.size()-1; depth >= 0; depth--) {
      for(vertex_id_t u : verts_at_depth[depth]) {
    for(auto edge : out_neigh[u]) {
      vertex_id_t v = std::get<0>(edge);
      if(depths[v] == depths[u] + 1) {
        deltas[u] += static_cast<double>(path_counts[u]) /
          static_cast<double>(path_counts[v]) * (1 + deltas[v]);
      }
    }
    scores[u] += deltas[u];
      }
    }
  }

  score_t biggest_score = *std::max_element(scores.begin(), scores.end());
  for(size_t i = 0; i < g.max() + 1; ++i) {
    scores[i] = scores[i] / biggest_score;
  }

  bool all_ok = true;

  for(size_t i = 0; i < scores.size(); ++i) {
    accum_t delta = abs(scores_to_test[i] - scores[i]);
    if(delta > 1e-6) {
      std::cout << i << ": " << scores[i] << " != " << scores_to_test[i] << " " << scores[i] - scores_to_test[i] << std::endl;
      all_ok = false;
    }
  }

  return all_ok;
}

template <typename Graph, typename score_t = float, typename accum_t = size_t>
auto bc2_v0(Graph& graph, const std::vector<vertex_id_t> sources, bool normalize = true) {

  auto                 g = graph.begin();
  vertex_id_t          N = graph.max() + 1;
  std::vector<score_t> bc(N);

  for (vertex_id_t root : sources) {

    std::vector<vertex_id_t>                depths(N, std::numeric_limits<vertex_id_t>::max());
    std::vector<accum_t>                    path_counts(N);
    std::vector<std::multiset<vertex_id_t>> P(N);

    std::vector<std::deque<vertex_id_t>> S(2);

    path_counts[root] = 1;
    depths[root]      = 0;
    vertex_id_t phase = 0;
    vertex_id_t count = 1;

    S[phase].push_front(root);

    while (count > 0) {
      count = 0;

      for (vertex_id_t v : S[phase]) {
        for (auto&& x : g[v]) {
          auto w = std::get<0>(x);

          if (depths[w] == std::numeric_limits<vertex_id_t>::max()) {
            S[phase + 1].push_front(w);
            count     = count + 1;
            depths[w] = depths[v] + 1;
          }

          if (depths[w] == depths[v] + 1) {
            path_counts[w] = path_counts[w] + path_counts[v];
            P[w].insert(v);
          }
        }
      }

      phase = phase + 1;
      S.push_back(std::deque<vertex_id_t>());
    }

    std::vector<score_t> delta(N);

    while (phase-- > 0) {
      for (auto w : S[phase]) {
        for (auto v : P[w]) {
          delta[v] += static_cast<score_t>(path_counts[v]) / static_cast<score_t>(path_counts[w]) * (1 + delta[w]);
        }
        bc[w] = bc[w] + delta[w];
      }
    }
  }
  if (normalize) {
    score_t biggest_score = *max_element(bc.begin(), bc.end());
    for (auto &j : bc)
      j = j / biggest_score; 
  }

  return bc;
}

template <typename Graph, typename score_t = float, typename accum_t = size_t>
auto bc2_v1(Graph& graph, const std::vector<vertex_id_t> sources, bool normalize = true) {

  auto                 g = graph.begin();
  vertex_id_t          N = graph.max() + 1;
  std::vector<score_t> bc(N);

  for (vertex_id_t root : sources) {

    std::vector<std::atomic<vertex_id_t>> depths(N);
    std::for_each(depths.begin(), depths.end(),
                  [&](std::atomic<vertex_id_t>& x) { x.store(std::numeric_limits<vertex_id_t>::max()); });
    std::vector<accum_t>                    path_counts(N);
    std::vector<std::multiset<vertex_id_t>> P(N);

    std::vector<tbb::concurrent_vector<vertex_id_t>> S(2);

    path_counts[root]              = 1;
    depths[root]                   = 0;
    vertex_id_t              phase = 0;
    std::atomic<vertex_id_t> count = 1;

    S[phase].push_back(root);

    while (count > 0) {
      count = 0;

      for (vertex_id_t v : S[phase]) {
        for (auto&& [w] : g[v]) {

          vertex_id_t neg_one = std::numeric_limits<vertex_id_t>::max();
          if (depths[w].compare_exchange_strong(neg_one, depths[v] + 1)) {
            S[phase + 1].push_back(w);
            ++count;
          }
          if (depths[w] == depths[v] + 1) {
            path_counts[w] += path_counts[v];
            P[w].insert(v);
          }
        }
      }

      phase = phase + 1;
      S.push_back(tbb::concurrent_vector<vertex_id_t>());
    }

    std::vector<score_t> delta(N);

    while (--phase > 0) {
      for (auto w : S[phase]) {
        for (auto v : P[w]) {
          delta[v] += static_cast<score_t>(path_counts[v]) / static_cast<score_t>(path_counts[w]) * (1 + delta[w]);
        }
        bc[w] = bc[w] + delta[w];
      }
    }
  }

  if (normalize) {
    score_t biggest_score = *max_element(bc.begin(), bc.end());
    for (auto &j : bc)
      j = j / biggest_score; 
    j = j / biggest_score;
      j = j / biggest_score; 
  }

  return bc;
}

template <typename Graph, typename score_t = float, typename accum_t = size_t, class ExecutionPolicy = std::execution::parallel_unsequenced_policy>
auto bc2_v2(Graph& graph, const std::vector<vertex_id_t>& sources, ExecutionPolicy&& policy = {}, bool normalize = true) {

  auto                 g = graph.begin();
  vertex_id_t          N = graph.max() + 1;
  std::vector<score_t> bc(N, 0);

  for (vertex_id_t root : sources) {
    std::vector<std::atomic<vertex_id_t>> depths(N);

    std::fill(policy, depths.begin(), depths.end(), std::numeric_limits<vertex_id_t>::max());

    std::vector<accum_t> path_counts(N, 0);

    std::vector<tbb::concurrent_vector<vertex_id_t>> P(N);
    std::vector<tbb::concurrent_vector<vertex_id_t>> S(2);

    vertex_id_t depth = 0;

    path_counts[root]              = 1;
    depths[root]                   = depth++;
    vertex_id_t              phase = 0;
    std::atomic<vertex_id_t> count = 1;

    S[phase].push_back(root);

    while (count > 0) {
      count = 0;

      std::for_each(policy, S[phase].begin(), S[phase].end(), [&](auto&& v) {
        tbb::parallel_for(g[v], [&](auto&& elt) {
          for (auto&& [w] : elt) {

            vertex_id_t neg_one = std::numeric_limits<vertex_id_t>::max();
            if ((depths[w] == neg_one) && depths[w].compare_exchange_strong(neg_one, depth)) {
              S[phase + 1].push_back(w);
              ++count;
            }
            if (depths[w] == depth) {
              path_counts[w] += path_counts[v];
              P[w].push_back(v);
            }
          }
        });
      });

      phase = phase + 1;
      S.push_back(tbb::concurrent_vector<vertex_id_t>());
      ++depth;
    }

    std::cout << "depth phase = " << depth << " " << phase << std::endl;

    std::vector<std::atomic<score_t>> delta(N);

    while (--phase > 0) {
      std::for_each(policy, S[phase].begin(), S[phase].end(), [&](auto&& w) {
        std::for_each(P[w].begin(), P[w].end(), [&](auto&& v) {
          delta[v] = delta[v] + static_cast<score_t>(path_counts[v]) / static_cast<score_t>(path_counts[w]) * (1 + delta[w]);
        });

        bc[w] += delta[w];
      });
    }
  }
  if (normalize) {
    score_t biggest_score = *max_element(policy, bc.begin(), bc.end());
    std::for_each(policy, bc.begin(), bc.end(), [&](score_t& j) { j = j / biggest_score; });
  }
  return bc;
}

template <typename Graph, typename score_t = float, typename accum_t = size_t, class OuterExecutionPolicy = std::execution::parallel_unsequenced_policy, class InnerExecutionPolicy = std::execution::parallel_unsequenced_policy>
auto bc2_v3(Graph& graph, const std::vector<vertex_id_t>& sources, OuterExecutionPolicy&& outer_policy = {}, InnerExecutionPolicy&& inner_policy = {}, bool normalize = true) {

  auto        g = graph.begin();
  vertex_id_t N = graph.max() + 1;
  size_t      M = graph.to_be_indexed_.size();

  auto                 neighbors = (*(graph.begin())).begin();
  std::vector<score_t> bc(N);

  const vertex_id_t num_bins = 32;
  const vertex_id_t bin_mask = 0x1F;

  for (vertex_id_t root : sources) {
    std::cout << "source: " << root << std::endl;

    std::vector<std::atomic<vertex_id_t>> levels(N);
    std::vector<std::atomic<bool>>        succ(M);    // use tbb:: bit map ?

    std::fill(outer_policy, levels.begin(), levels.end(), std::numeric_limits<vertex_id_t>::max());

    std::vector<std::atomic<accum_t>>                             path_counts(N);    // move outside loop?
    std::vector<tbb::concurrent_vector<vertex_id_t>>              q1(num_bins);
    std::vector<tbb::concurrent_vector<vertex_id_t>>              q2(num_bins);
    std::vector<std::vector<tbb::concurrent_vector<vertex_id_t>>> retired;

    vertex_id_t lvl = 0;

    path_counts[root] = 1;
    q1[0].push_back(root);
    levels[root] = lvl++;

    bool done = false;
    while (!done) {
      std::for_each(outer_policy, q1.begin(), q1.end(), [&](auto& q) {
        std::for_each(inner_policy, q.begin(), q.end(), [&](vertex_id_t u) {
          // tbb::parallel_for(g[u], [&](auto&& gu) {
          auto gu = g[u];
          for (auto x = gu.begin(); x != gu.end(); ++x) {
            auto&&      v       = std::get<0>(*x);
            vertex_id_t neg_one = std::numeric_limits<vertex_id_t>::max();
            if (levels[v] == neg_one && levels[v].compare_exchange_strong(neg_one, lvl)) {
              q2[u & bin_mask].push_back(v);
            }
            if (levels[v] == lvl) {
              // path_counts[v].fetch_add(path_counts[u]);
              for (accum_t temp = path_counts[v];
                   !path_counts[v].compare_exchange_strong(temp, temp + path_counts[u], std::memory_order_acq_rel);)
                ;
              succ[x - neighbors] = true;    // edge(w,v) : P[w][v]
            }
          }
          //});
        });
      });

      done = true;
      for (size_t i = 0; i < num_bins; ++i) {
        if (q2[i].size() != 0) {
          done = false;
          break;
        }
      }

      retired.emplace_back(num_bins);
      std::swap(q1, retired.back());
      std::swap(q1, q2);

      ++lvl;
    }

    // std::cout << "lvl = " << lvl << " " << retired.size() << std::endl;

    std::vector<score_t> deltas(N);

    std::for_each(retired.rbegin(), retired.rend(), [&](auto&& vvv) {
      std::for_each(outer_policy, vvv.begin(), vvv.end(), [&](auto&& vv) {
        std::for_each(inner_policy, vv.begin(), vv.end(), [&](auto&& u) {
          score_t delta = 0;
          for (auto x = g[u].begin(); x != g[u].end(); ++x) {
            vertex_id_t v = std::get<0>(*x);
            if (succ[x - neighbors]) {
              delta += static_cast<double>(path_counts[u]) / static_cast<double>(path_counts[v]) * (1 + deltas[v]);
            }
          }
          deltas[u] = delta;
          bc[u] += delta;
        });
      });
    });
  }
  if (normalize) {
    score_t biggest_score = *max_element(outer_policy, bc.begin(), bc.end());
    std::for_each(outer_policy, bc.begin(), bc.end(), [&](score_t& j) { j = j / biggest_score; });
  }
  return bc;
}

template <class score_t, class accum_t, class Graph, class OuterExecutionPolicy = std::execution::parallel_unsequenced_policy, class InnerExecutionPolicy = std::execution::parallel_unsequenced_policy>
auto bc2_v4(Graph&& graph, const std::vector<vertex_id_t>& sources, int threads, OuterExecutionPolicy&& outer_policy={}, InnerExecutionPolicy&& inner_policy={}, bool normalize = true) {
  auto                 g     = graph.begin();
  vertex_id_t          N     = graph.max() + 1;
  size_t               M     = graph.to_be_indexed_.size();
  auto&&               edges = std::get<0>(*(*g).begin());
  std::vector<score_t> bc(N);

  const vertex_id_t num_bins = nw::graph::pow2(nw::graph::ceil_log2(threads));
  const vertex_id_t bin_mask = num_bins - 1;

  for (vertex_id_t root : sources) {
    std::vector<std::atomic<vertex_id_t>> levels(N);
    nw::graph::AtomicBitVector                succ(M);

    std::fill(outer_policy, levels.begin(), levels.end(), std::numeric_limits<vertex_id_t>::max());

    std::vector<std::atomic<accum_t>>                             path_counts(N);    // move outside loop?
    std::vector<tbb::concurrent_vector<vertex_id_t>>              q1(num_bins);
    std::vector<tbb::concurrent_vector<vertex_id_t>>              q2(num_bins);
    std::vector<std::vector<tbb::concurrent_vector<vertex_id_t>>> retired;

    vertex_id_t lvl = 0;

    path_counts[root] = 1;
    q1[0].push_back(root);
    levels[root] = lvl++;

    bool done = false;
    while (!done) {
      std::for_each(outer_policy, q1.begin(), q1.end(), [&](auto&& q) {
        std::for_each(inner_policy, q.begin(), q.end(), [&](auto&& u) {
          for (auto x = g[u].begin(); x != g[u].end(); ++x) {
            auto&& v = std::get<0>(*x);
            auto&& neg_one = std::numeric_limits<vertex_id_t>::max();
            if (nw::graph::acquire(levels[v]) == neg_one && nw::graph::cas(levels[v], neg_one, lvl)) {
              q2[u & bin_mask].push_back(v);
            }
            if (nw::graph::acquire(levels[v]) == lvl) {
              nw::graph::fetch_add(path_counts[v], nw::graph::acquire(path_counts[u]));
              succ.atomic_set(&v - &edges);    // edge(w,v) : P[w][v]
            }
          }
        });
      });

      done = true;
      for (size_t i = 0; i < num_bins; ++i) {
        if (q2[i].size() != 0) {
          done = false;
          break;
        }
      }

      retired.emplace_back(num_bins);
      std::swap(q1, retired.back());
      std::swap(q1, q2);

      ++lvl;
    }

    // std::cout << "lvl = " << lvl << " " << retired.size() << std::endl;

    std::vector<score_t> deltas(N);

    std::for_each(retired.rbegin(), retired.rend(), [&](auto&& vvv) {
      std::for_each(outer_policy, vvv.begin(), vvv.end(), [&](auto&& vv) {
        std::for_each(inner_policy, vv.begin(), vv.end(), [&](auto&& u) {
          score_t delta = 0;
          for (auto x = g[u].begin(); x != g[u].end(); ++x) {
            auto&& v = std::get<0>(*x);
            if (succ.get(&v - &edges)) {
              delta += path_counts[u] / path_counts[v] * (1 + deltas[v]);
            }
          }
          deltas[u] = delta;
          bc[u] += delta;
        });
      });
    });
  }

  if (normalize) {
    score_t biggest_score = *max_element(outer_policy, bc.begin(), bc.end());
    std::for_each(outer_policy, bc.begin(), bc.end(), [&](score_t& j) { j = j / biggest_score; });
  }
  return bc;
}

template <class score_t, class accum_t, class Graph, class OuterExecutionPolicy = std::execution::parallel_unsequenced_policy, class InnerExecutionPolicy = std::execution::parallel_unsequenced_policy>
auto bc2_v5(Graph&& graph, const std::vector<vertex_id_t>& sources, int threads, OuterExecutionPolicy&& outer_policy = {}, InnerExecutionPolicy&& inner_policy = {}, bool normalize = true) {
  vertex_id_t          N     = graph.max() + 1;
  size_t               M     = graph.to_be_indexed_.size();
  auto&&               edges = std::get<0>(*(graph[0]).begin());
  std::vector<score_t> bc(N);

  const vertex_id_t num_bins = nw::graph::pow2(nw::graph::ceil_log2(threads));
  const vertex_id_t bin_mask = num_bins - 1;

  std::vector<std::future<void>> futures(sources.size());
  for (size_t s_idx = 0; s_idx < sources.size(); ++s_idx) {
    futures[s_idx] = std::async(
        std::launch::async,
        [&](vertex_id_t root) {
          std::vector<vertex_id_t> levels(N);
          nw::graph::AtomicBitVector   succ(M);

          // Initialize the levels to infinity.
          std::fill(outer_policy, levels.begin(), levels.end(), std::numeric_limits<vertex_id_t>::max());

          std::vector<accum_t>                                          path_counts(N);
          std::vector<tbb::concurrent_vector<vertex_id_t>>              q1(num_bins);
          std::vector<tbb::concurrent_vector<vertex_id_t>>              q2(num_bins);
          std::vector<std::vector<tbb::concurrent_vector<vertex_id_t>>> retired;

          vertex_id_t lvl = 0;

          path_counts[root] = 1;
          q1[0].push_back(root);
          levels[root] = lvl++;

          bool done = false;
          while (!done) {
            std::for_each(outer_policy, q1.begin(), q1.end(), [&](auto&& q) {
              std::for_each(inner_policy, q.begin(), q.end(), [&](auto&& u) {
                for (auto&& i : graph[u]) {
                  auto v = std::get<0>(i);
                  auto&& infinity = std::numeric_limits<vertex_id_t>::max();
                  auto&& lvl_v    = nw::graph::acquire(levels[v]);

                  // If this is our first encounter with this node, or
                  // it's on the right level, then propagate the counts
                  // from u to v, and mark the edge from u to v as used.
                  if (lvl_v == infinity || lvl_v == lvl) {
                    nw::graph::fetch_add(path_counts[v], nw::graph::acquire(path_counts[u]));
                    succ.atomic_set(&v - &edges);    // edge(w,v) : P[w][v]
                  }

                  // We need to add v to the frontier exactly once the
                  // first time we encounter it, so we race to set its
                  // level and if we win that race we can be the one to
                  // add it.
                  if (lvl_v == infinity && nw::graph::cas(levels[v], infinity, lvl)) {
                    q2[u & bin_mask].push_back(v);
                  }
                }
              });
            });

            done = true;
            for (size_t i = 0; i < num_bins; ++i) {
              if (q2[i].size() != 0) {
                done = false;
                break;
              }
            }

            retired.emplace_back(num_bins);
            std::swap(q1, retired.back());
            std::swap(q1, q2);

            ++lvl;
          }

          std::vector<score_t> deltas(N);

          std::for_each(retired.rbegin(), retired.rend(), [&](auto&& vvv) {
            std::for_each(outer_policy, vvv.begin(), vvv.end(), [&](auto&& vv) {
              std::for_each(inner_policy, vv.begin(), vv.end(), [&](auto&& u) {
                score_t delta = 0;
                for (auto&& i : graph[u]) {
                  auto v = std::get<0>(i);
                  if (succ.get(&v - &edges)) {
                    delta += path_counts[u] / path_counts[v] * (1.0f + deltas[v]);
                  }
                }
                nw::graph::fetch_add(bc[u], deltas[u] = delta);
              });
            });
          });
        },
        sources[s_idx]);
  }

  for (auto&& f : futures) {
    f.wait();
  }
  if (normalize) {
    auto max = std::reduce(outer_policy, bc.begin(), bc.end(), 0.0f, nw::graph::max{});
    std::for_each(outer_policy, bc.begin(), bc.end(), [&](auto&& j) { j /= max; });
  }
  return bc;
}

}    // namespace graph
}    // namespace nw
#endif    // BETWEENNESS_CENTRALITY_HPP
