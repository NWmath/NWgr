# -----------------------------------------------------------------------------
# The date library from MIT provides C++20 std date functionality in C++17
# -----------------------------------------------------------------------------
include(Date)
# find_package(Filesystem REQUIRED Final Experimental)
# message(STATUS ${CXX_FILESYSTEM_IS_EXPERIMENTAL})

# -----------------------------------------------------------------------------
# Generate the configuration files used for logging, and -I its built path.
# -----------------------------------------------------------------------------
get_filename_component(NW_GRAPH_CXX_COMPILER ${CMAKE_CXX_COMPILER} NAME)
configure_file(config.h.in config.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# -----------------------------------------------------------------------------
# Create an interface library for the benchmarks to depend on.
# -----------------------------------------------------------------------------
add_library(bench INTERFACE)
target_sources(bench INTERFACE
  common.hpp
  Log.hpp
  #config.h
  )
target_include_directories(bench INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(bench INTERFACE nw_graph docopt date::date nlohmann_json::nlohmann_json) 

find_package(nlohmann_json 3.2.0 QUIET)
if (NOT nlohmann_json_FOUND)
  message("Installed json not found -- fetching")
  FetchContent_Declare(json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.7.3)

  FetchContent_GetProperties(json)
  if(NOT json_POPULATED)
    FetchContent_Populate(json)
    add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
  endif()
endif()


# -----------------------------------------------------------------------------
# NW_GRAPH driver to process edgelists.
# -----------------------------------------------------------------------------
add_executable(process_edge_list ProcessEdgelist.cpp)
target_link_libraries(process_edge_list nw_graph docopt )

# -----------------------------------------------------------------------------
# NW_GRAPH driver to verify that we're correctly generating source lists.
# -----------------------------------------------------------------------------
#set policy to suppress warnings
cmake_policy(SET CMP0115 NEW)
add_executable(sources SourceVerifier.cpp)
target_link_libraries(sources bench)

add_executable(bc.exe bc.cpp)
target_link_libraries(bc.exe bench)

add_executable(bfs.exe bfs.cpp)
target_link_libraries(bfs.exe bench)

add_executable(cc.exe cc.cpp) 
target_link_libraries(cc.exe bench) 

# add_executable(js.exe js.cpp)
# target_link_libraries(js.exe bench)

add_executable(pr.exe pr.cpp)
target_link_libraries(pr.exe bench)

add_executable(sssp.exe sssp.cpp)
target_link_libraries(sssp.exe bench)

add_executable(tc.exe tc.cpp)
target_link_libraries(tc.exe bench nlohmann_json::nlohmann_json)


