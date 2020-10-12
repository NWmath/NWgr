
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


add_executable(imdb.exe imdb.cpp)
target_link_libraries(imdb.exe nw_graph xtensor)

add_executable(oracle.exe oracle.cpp)
target_link_libraries(oracle.exe nw_graph nlohmann_json::nlohmann_json)

