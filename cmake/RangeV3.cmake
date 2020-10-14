# -----------------------------------------------------------------------------
# Allow our executables to use ranges.
# -----------------------------------------------------------------------------
include(FetchContent)

FetchContent_Declare(
  range-v3
  GIT_REPOSITORY https://github.com/ericniebler/range-v3
  GIT_TAG        0.10.0)
set(RANGES_CXX_STD 17 CACHE STRING "C++ standard version." FORCE)
FetchContent_MakeAvailable(range-v3)
