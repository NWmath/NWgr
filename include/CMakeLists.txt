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

include(FetchContent)

FetchContent_Declare(nwut
  GIT_REPOSITORY     git@github.com:NWmath/NWut.git
  GIT_TAG            master
  # URL ${CMAKE_SOURCE_DIR}/contrib/nwut-something.zip
  # URL_MD5 HASH_FOR_ZIP^
  )
FetchContent_MakeAvailable(nwut)

add_library(nw_graph INTERFACE)

# Require C++20
target_compile_features(nw_graph INTERFACE cxx_std_20)

# Add an alias for external uses
add_library(nw::graph ALIAS nw_graph)

# make sure we have the right include path
target_include_directories(nw_graph INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

# Extend this as needed.
target_sources(nw_graph
  INTERFACE
  adaptors/dag_range.hpp
  adaptors/random_range.hpp
  containers/soa.hpp
  adaptors/bfs_range.hpp
  containers/edge_list.hpp
  util/print_types.hpp
  adaptors/bfs_edge_range.hpp
  adaptors/back_edge_range.hpp
  adaptors/neighbor_range.hpp
  io/mmio.hpp
  adaptors/new_dfs_range.hpp
  containers/vovos.hpp
  adaptors/worklist.hpp
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
  algorithms/kruskal.hpp
  algorithms/prim.hpp
  algorithms/boykov_kolmogorov.hpp
  algorithms/max_flow.hpp
  util/provenance.hpp
  build.hpp
  graph_base.hpp
  util/disjoint_set.hpp
  adaptors/dfs_range.hpp
  util/proxysort.hpp
  containers/compressed.hpp
  containers/volos.hpp
  containers/adjacency.hpp
  util/util_par.hpp
  adaptors/reverse.hpp
  adaptors/plain_range.hpp
  adaptors/filtered_bfs_range.hpp
  containers/aos.hpp
  util/util.hpp
  detail/numeric.hpp
  adaptors/edge_range.hpp)

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

# find_package(TBB)
# if (not TBB_Found)
#   FetchContent(
#     TBB_STUFF)
# endif()

find_package(Threads REQUIRED)
target_compile_definitions(nw_graph INTERFACE EXECUTION_POLICY)
target_link_libraries(nw_graph INTERFACE TBB::tbb Threads::Threads nw::util)
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

