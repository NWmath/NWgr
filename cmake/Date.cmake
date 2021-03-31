# -----------------------------------------------------------------------------
# Allow our executables to use docopt.
# -----------------------------------------------------------------------------
include(FetchContent)

if(MSVC)
  set(COMPILE_WITH_C_LOCALE ON)
endif()

FetchContent_Declare(
  date
  GIT_REPOSITORY https://github.com/HowardHinnant/date.git
  GIT_TAG master)

FetchContent_MakeAvailable(date)

if(NOT MSVC)
  target_compile_options(date INTERFACE -Wno-deprecated-declarations)
endif()