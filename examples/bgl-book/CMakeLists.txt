
# find_package
# fetch_content
# external project add

# set(CMAKE_CXX_CLANG_TIDY clang-tidy -checks=-*,readability-*)

add_executable(ch3_toposort.exe ch3_toposort.cpp)
target_link_libraries(ch3_toposort.exe nw_graph)

add_executable(ch4_kevin_bacon.exe ch4_kevin_bacon.cpp)
target_link_libraries(ch4_kevin_bacon.exe nw_graph)

add_executable(ch4_loop_detection.exe ch4_loop_detection.cpp)
target_link_libraries(ch4_loop_detection.exe nw_graph)

add_executable(ch5_bellman_ford.exe ch5_bellman_ford.cpp)
target_link_libraries(ch5_bellman_ford.exe nw_graph)

add_executable(ch5_dijkstra.exe ch5_dijkstra.cpp)
target_link_libraries(ch5_dijkstra.exe nw_graph)

add_executable(ch6_kruskal.exe ch6_kruskal.cpp)
target_link_libraries(ch6_kruskal.exe nw_graph)

add_executable(ch6_prim.exe ch6_prim.cpp)
target_link_libraries(ch6_prim.exe nw_graph)

add_executable(ch7_connected.exe ch7_connected.cpp)
target_link_libraries(ch7_connected.exe nw_graph)

add_executable(ch7_strongly_connected.exe ch7_strongly_connected.cpp)
target_link_libraries(ch7_strongly_connected.exe nw_graph)

add_executable(ch8_maxflow.exe ch8_maxflow.cpp)
target_link_libraries(ch8_maxflow.exe nw_graph)

add_executable(ch9_knights_tour.exe ch9_knights_tour.cpp)
target_link_libraries(ch9_knights_tour.exe nw_graph)

