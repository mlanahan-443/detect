# Unit tests with Catch2 v3.
#
# FetchContent is the closest thing CMake has to `uv add`: it downloads the
# dependency at configure time and builds it from source as part of this
# project. FIND_PACKAGE_ARGS (CMake 3.24+) first tries an installed Catch2
# and only downloads if none is found.
include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.16.1 # pin an exact tag, like a lockfile entry
  GIT_SHALLOW TRUE
  FIND_PACKAGE_ARGS 3)
FetchContent_MakeAvailable(Catch2)

# The lint preset sets CMAKE_CXX_CLANG_TIDY globally, which would also lint
# Catch2's own sources. Switch it off for the dependency's targets (they only
# exist when Catch2 was built from source rather than found installed).
foreach(_dep_target IN ITEMS Catch2 Catch2WithMain)
  if(TARGET ${_dep_target})
    set_target_properties(${_dep_target} PROPERTIES CXX_CLANG_TIDY "")
  endif()
endforeach()

enable_testing()
add_executable(detect_tests test_version.cpp test_array.cpp test_kernel.cpp test_convolve.cpp)
target_link_libraries(detect_tests PRIVATE detect::detect detect::options
                                              Catch2::Catch2WithMain)

# Registers every TEST_CASE with CTest individually, so `ctest -R name`
# works like `pytest -k name`.
include(Catch)
catch_discover_tests(detect_tests)
