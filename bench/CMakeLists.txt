# -----------------------------------------------------------------------------
# The date library from MIT provides C++20 std date functionality in C++17
# -----------------------------------------------------------------------------
include(Date)
find_package(Filesystem REQUIRED Final Experimental)
message(STATUS ${CXX_FILESYSTEM_IS_EXPERIMENTAL})

# -----------------------------------------------------------------------------
# Generate the configuration files used for logging, and -I its built path.
# -----------------------------------------------------------------------------
get_filename_component(BGL17_CXX_COMPILER ${CMAKE_CXX_COMPILER} NAME)
configure_file(config.h.in config.h)

# -----------------------------------------------------------------------------
# Create an interface library for the benchmarks to depend on.
# -----------------------------------------------------------------------------
add_library(bench INTERFACE)
target_sources(bench INTERFACE
  common.hpp
  Log.hpp
  config.h)
target_include_directories(bench INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(bench INTERFACE bgl docopt date::date std::filesystem)

# -----------------------------------------------------------------------------
# BGL17 driver to process edgelists.
# -----------------------------------------------------------------------------
add_executable(process_edge_list ProcessEdgelist.cpp)
target_link_libraries(process_edge_list bgl docopt)

# -----------------------------------------------------------------------------
# BGL17 driver to verify that we're correctly generating source lists.
# -----------------------------------------------------------------------------
add_executable(sources SourceVerifier.cpp)
target_link_libraries(sources bench)

add_executable(bfs.exe bfs.cpp)
target_link_libraries(bfs.exe bench)

add_executable(pr.exe pr.cpp)
target_link_libraries(pr.exe bench)

add_executable(sssp.exe sssp.cpp)
target_link_libraries(sssp.exe bench)

add_executable(sv.exe sv.cpp)
target_link_libraries(sv.exe bgl)

add_executable(tc.exe tc.cpp)
target_link_libraries(tc.exe bench)

add_executable(bc.exe bc.cpp)
target_link_libraries(bc.exe bench)

add_executable(cc.exe cc.cpp) 
target_link_libraries(cc.exe bench) 

add_executable(bicc.exe bicc.cpp) 
target_link_libraries(bicc.exe bench) 

add_executable(bicc2.exe bicc2.cpp)
target_link_libraries(bicc2.exe bench)

