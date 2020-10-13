
# find_package
# fetch_content
# 
include(FetchContent)

# set(XTENSOR_USE_XSIMD 1)
set(XTENSOR_USE_TBB 1)

# xsimd and TBB dependencies are automatically
# searched when the following is executed
find_package(xtensor REQUIRED)

#find_package(nlohmann_json 3.2.0 REQUIRED)


FetchContent_Declare(json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.7.3)

FetchContent_GetProperties(json)
if(NOT json_POPULATED)
  FetchContent_Populate(json)
  add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()


find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(nw_graph INTERFACE OpenMP::OpenMP_CXX)
endif()



add_executable(imdb.exe imdb.cpp)
target_link_libraries(imdb.exe nw_graph xtensor)

add_executable(dns.exe dns.cpp)
target_link_libraries(dns.exe nw_graph xtensor)

add_executable(oracle.exe oracle.cpp)
target_link_libraries(oracle.exe nw_graph nlohmann_json::nlohmann_json)

add_executable(dblp.exe dblp.cpp)
target_link_libraries(dblp.exe nw_graph nlohmann_json::nlohmann_json)

add_executable(oracle+dblp.exe oracle+dblp.cpp)
target_link_libraries(oracle+dblp.exe nw_graph nlohmann_json::nlohmann_json)

