# -----------------------------------------------------------------------------
# Define the primary NW_GRAPH target.
# -----------------------------------------------------------------------------
#
# This target keeps track of all of the header sources for NW_GRAPH, and sets up all
# of the associated flags and dependencies such that we can correctly compile
# and link executables that are dependent on NW_GRAPH. Ultimately this target will
# control the install target too, once we have one.
#
# Executable targets should be defined with `target_link_libraries(exec nw_graph)`
# and then they'll have the appropriate `-I`, `-D`, and `-l` stuff set
# automagically though the transitive dependency infrastructure.

# @note
#   If NW_GRAPH starts to have .cpp sources, change this from an interface library to
#   a STATIC (or DYNAMIC) library, and add those sources here. Change other
#   instances of INTERFACE to PUBLIC.
# @note
add_library(nw_graph INTERFACE)

# Extend this as needed.
target_sources(nw_graph
  INTERFACE
  dag_range.hpp
  random_range.hpp
  soa.hpp
  bfs_range.hpp
  edge_list.hpp
  util/print_types.hpp
  util/types.hpp
  bfs_edge_range.hpp
  back_edge_range.hpp
  neighbor_range.hpp
  io/mmio.hpp
  new_dfs_range.hpp
  vovos.hpp
  worklist.hpp
  algorithms/k_core.hpp
  algorithms/mis.hpp
  algorithms/triangle_count.hpp
  algorithms/connected_components.hpp
  algorithms/page_rank.hpp
  algorithms/betweenness_centrality.hpp
  algorithms/dijkstra.hpp
  algorithms/jones_plassmann_coloring.hpp
  algorithms/bfs.hpp
  algorithms/spMatspMat.hpp
  algorithms/dag_based_mis.hpp
  algorithms/delta_stepping.hpp
  algorithms/spanning_tree.hpp
  algorithms/boykov_kolmogorov.hpp
  algorithms/max_flow.hpp
  provenance.hpp
  graph_base.hpp
  disjoint_set.hpp
  dfs_range.hpp
  proxysort.hpp
  compressed.hpp
  aolos.hpp
  util/util_par.hpp
  reverse.hpp
  plain_range.hpp
  filtered_bfs_range.hpp
  aos.hpp
  util/util.hpp
  detail/numeric.hpp
  edge_range.hpp)

target_include_directories(nw_graph INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(nw_graph INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/../../NWut/include)

# -----------------------------------------------------------------------------
# Check to see if our C++ compiler supports Parallelism TS. If it does than
# nothing weird will happen. If it doesn't we define the `NEED_` macros and that
# will pull in `detail/numeric.hpp`.
# -----------------------------------------------------------------------------
include(CheckCXXSourceCompiles)
check_cxx_source_compiles(
  "#include<numeric>
    int main() {
        int* ptr;
        std::inclusive_scan(ptr, ptr, ptr);
    }"
  HAVE_INCLUSIVE_SCAN)

if (NOT HAVE_INCLUSIVE_SCAN)
  target_compile_definitions(nw_graph INTERFACE NW_GRAPH_NEED_INCLUSIVE_SCAN)
endif ()

check_cxx_source_compiles(
  "#include<numeric>
    int main() {
        int* ptr;
        std::exclusive_scan(ptr, ptr, ptr, 0);
    }"
  HAVE_EXCLUSIVE_SCAN)

if (NOT HAVE_EXCLUSIVE_SCAN)
  target_compile_definitions(nw_graph INTERFACE NW_GRAPH_NEED_EXCLUSIVE_SCAN)
endif ()

check_cxx_source_compiles(
  "#include<numeric>
    int main() {
        int* ptr;
        std::reduce(ptr, ptr, 0);
    }"
  HAVE_REDUCE)

if (NOT HAVE_REDUCE)
  target_compile_definitions(nw_graph INTERFACE NW_GRAPH_NEED_REDUCE)
endif ()

# -----------------------------------------------------------------------------
# Handle requests for parallel execution
# -----------------------------------------------------------------------------

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(TBB REQUIRED)
find_package(Threads REQUIRED)
target_compile_definitions(nw_graph INTERFACE EXECUTION_POLICY)
target_link_libraries(nw_graph INTERFACE TBB::tbb Threads::Threads)
if (NW_GRAPH_USE_TBBMALLOC)
  target_link_libraries(nw_graph INTERFACE TBB::tbbmalloc) 
endif ()


# -----------------------------------------------------------------------------
# Handle Apple-specific things
# -----------------------------------------------------------------------------
if (APPLE)
  if (NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
    target_compile_definitions(nw_graph INTERFACE _Atomic=volatile)
  endif()
endif()
