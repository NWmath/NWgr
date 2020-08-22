##########################################################################
#	CMake Build Rules for the NW Graph (v0.3)                              #
##########################################################################
# Basic Usage:                                                           #
#   cmake ..								 #
#   make  								 #
#   make test                                                            #
# For more information about CMake, see http://www.cmake.org             #
##########################################################################
#
# Useful things to know 
# make VERBOSE=1
# cmake --trace
# cmake .. -DCMAKE_CXX_COMPILER=icpx
#
##########################################################################
cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

set(NW_GRAPH_VERSION 0.3)

project(
  NW_GRAPH
  VERSION ${NW_GRAPH_VERSION}
  LANGUAGES CXX)

# -----------------------------------------------------------------------------
# Extend the module path so we can find our custom modules
# -----------------------------------------------------------------------------
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# -----------------------------------------------------------------------------
# Build types and default flags
# -----------------------------------------------------------------------------
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -fno-elide-constructors -fconcepts-diagnostics-depth=3" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS_RELEASE "-fconcepts-diagnostics-depth=3 -Ofast -march=native -DNDEBUG" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-fconcepts-diagnostics-depth=3 -Ofast -g -march=native" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS_MINSIZEREL "-fconcepts-diagnostics-depth=3 -Os -march=native -DNDEBUG" CACHE STRING "" FORCE)

# Default to Release build
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "No build type selected, default to Release")
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type (default Release)" FORCE)
endif()

# Control compiler-specific flags.
include(CompilerFlags)

# -----------------------------------------------------------------------------
# Select a C++ standard
# -----------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# -----------------------------------------------------------------------------
# Options
# -----------------------------------------------------------------------------
set(NW_GRAPH_SANITIZE "" CACHE STRING "-fsanitize compiler options (e.g., =address)")
if (NW_GRAPH_SANITIZE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer -fsanitize=${NW_GRAPH_SANITIZE}")
  set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=${NW_GRAPH_SANITIZE}")
endif (NW_GRAPH_SANITIZE)

option(NW_GRAPH_EXECUTION_POLICY "with(out) C++ parallel execution policy" ON)
option(NW_GRAPH_BUILD_TESTS "Determines whether to build tests." ON)
option(NW_GRAPH_BUILD_APBS "Determines whether to build abstraction penalty benchmarks." OFF)
option(NW_GRAPH_BUILD_BENCH "Determines whether to build performance benchmarks." ON)
option(NW_GRAPH_USE_TBBMALLOC "Link to tbbmalloc" OFF)

add_subdirectory(include)

include(Docopt)
include(RangeV3)
  
if (NW_GRAPH_BUILD_TESTS)
  include(CTest)
  add_subdirectory(test)
endif()

if (NW_GRAPH_BUILD_APBS)
  add_subdirectory(apb)
endif()

if (NW_GRAPH_BUILD_BENCH)
  add_subdirectory(bench)
endif()
