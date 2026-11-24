# Top-level build description. Think of it as pyproject.toml + the build
# backend in one file: it names the project, declares options, and pulls in
# the subdirectories that define the actual targets.

cmake_minimum_required(VERSION 3.25)

project(
  detect
  VERSION 0.1.0
  DESCRIPTION "Detect-and-track pipeline: NumPy prototype rebuilt in C++"
  LANGUAGES CXX)

# Refuse in-source builds: all generated files belong under build/.
if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(FATAL_ERROR "In-source builds are not allowed. Use a preset: cmake --preset debug")
endif()

# ---- Options (set from CMakePresets.json or -D on the command line) --------
# PROJECT_IS_TOP_LEVEL is false if someone pulls this repo in as a dependency,
# so they don't inherit our tests or our -Werror.
option(DETECT_BUILD_TESTS "Build the unit tests" ${PROJECT_IS_TOP_LEVEL})
option(DETECT_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" ${PROJECT_IS_TOP_LEVEL})
option(DETECT_ENABLE_SANITIZERS "Build with AddressSanitizer + UBSan" OFF)

# Defines the detect::options target that carries warnings/sanitizers.
include(cmake/ProjectOptions.cmake)

# ---- Targets ---------------------------------------------------------------
add_subdirectory(src)   # the library
add_subdirectory(apps)  # executables that use the library

if(DETECT_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()
