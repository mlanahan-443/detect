# The core library. Everything the pipeline does lives here; apps/ and
# tests/ are thin consumers of it.

add_library(detect)
# The namespaced alias is what consumers link against. A typo in an alias
# name is a configure error; a typo in a plain name silently becomes "-ltypo".
add_library(detect::detect ALIAS detect)

target_sources(
  detect
  PRIVATE version.cpp
  # FILE_SET HEADERS (CMake 3.23+) declares the public headers and sets the
  # include path in one step: consumers write #include <detect/version.hpp>.
  PUBLIC FILE_SET HEADERS
         BASE_DIRS ${PROJECT_SOURCE_DIR}/include
         FILES ${PROJECT_SOURCE_DIR}/include/detect/version.hpp)

# PUBLIC: anything linking detect is also compiled as C++23.
target_compile_features(detect PUBLIC cxx_std_23)
set_target_properties(detect PROPERTIES CXX_EXTENSIONS OFF) # -std=c++23, not gnu++23

# PRIVATE: the version string is an implementation detail of version.cpp.
target_compile_definitions(detect PRIVATE DETECT_VERSION="${PROJECT_VERSION}")

# PRIVATE: our warnings apply when building detect, not to whoever uses it.
target_link_libraries(detect PRIVATE detect::options)
