// tests/test_Array2d.cpp
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <stdexcept>

#include "detect/kernel2d.hpp"

using kernel2d::Kernel2d;


TEST_CASE("Instantiation with even rows or cols fails.","[Kernel2d]"){

    SECTION("fails with zeroded data"){
        REQUIRE_THROWS_AS(
            (Kernel2d<int>{4,5}),
            std::invalid_argument
        );
        REQUIRE_THROWS_AS(
            (Kernel2d<int>{5,4}),
            std::invalid_argument
        );
    }

    SECTION("fails with fill data"){
        REQUIRE_THROWS_AS(
            (Kernel2d<int>{4,5,2}),
            std::invalid_argument
        );
        REQUIRE_THROWS_AS(
            (Kernel2d<int>{5,4,2}),
            std::invalid_argument
        );
    }

    SECTION("fails with vector data"){

        std::size_t rows = 4;
        std::size_t cols = 5;
        std::size_t size = rows*cols;
    
        std::vector<int> data(size);
        int count{9};
        for (std::size_t r = 0; r < rows; ++r) {
            for (std::size_t c = 0; c < cols; ++c) {
                count +=1;
                data[r*cols + c] = count;
            }
        }

        REQUIRE_THROWS_AS(
            (Kernel2d<int>{4,5,data}),
            std::invalid_argument
        );
        REQUIRE_THROWS_AS(
            (Kernel2d<int>{5,4,data}),
            std::invalid_argument
        );
    }
}