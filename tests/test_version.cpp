#include <catch2/catch_test_macros.hpp>
#include <detect/version.hpp>

TEST_CASE("version string comes from CMake", "[smoke]") {
  REQUIRE(detect::version() == "0.1.0");
}
