# -----------------------------------------------------------------------------
# Allow our executables to use docopt.
# -----------------------------------------------------------------------------
include(FetchContent)

# work around bugs in the MSVC std library regex implementation
if(MSVC)
  set(USE_BOOST_REGEX ON)
endif()

FetchContent_Declare(
  docopt
  GIT_REPOSITORY https://github.com/docopt/docopt.cpp.git
  GIT_TAG master)

FetchContent_MakeAvailable(docopt)
