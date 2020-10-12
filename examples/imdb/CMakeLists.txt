
# find_package
# fetch_content
# 
include(FetchContent)

# set(XTENSOR_USE_XSIMD 1)
set(XTENSOR_USE_TBB 1)

# xsimd and TBB dependencies are automatically
# searched when the following is executed
find_package(xtensor REQUIRED)

add_executable(imdb.exe imdb.cpp)
target_link_libraries(imdb.exe nw_graph xtensor)

