// tests/test_Array2d.cpp
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <stdexcept>

#include "detect/array2d.hpp"
#include "detect/kernel2d.hpp"
#include "detect/image.hpp"

using array2d::Array2d;
using kernel2d::Kernel2d;

TEMPLATE_PRODUCT_TEST_CASE("{rows, cols} constructs a zeroed row-major buffer",
                        "[Array2d][ctor]",
                        (Array2d, Image, Kernel2d),
                        (int,double,float)) {

    TestType img{3, 5};   // odd dims, or Kernel2d throws

    SECTION("shape") {
        REQUIRE(img.rows() == 3);
        REQUIRE(img.cols() == 5);
    }
    SECTION("zeroed") {
        using T = std::remove_reference_t<decltype(img(0, 0))>;
        for (std::size_t r = 0; r < img.rows(); ++r)
            for (std::size_t c = 0; c < img.cols(); ++c)
                REQUIRE(img(r, c) == T{});
    }
}

TEMPLATE_PRODUCT_TEST_CASE("{rows, cols,value} constructs a fill-value row-major buffer",
    "[Array2d][ctor]",
    (Array2d, Image, Kernel2d),
    (int,float,double)) {

    TestType img{3, 5,5};   // odd dims, or Kernel2d throws

    using T = std::remove_reference_t<decltype(img(0, 0))>;
    T value{5};

    SECTION("fill value matches constructor arguments."){
        for (std::size_t r = 0; r < img.rows(); ++r) {
            for (std::size_t c = 0; c < img.cols(); ++c) {
                REQUIRE(img(r,c) == value);
            }
        }
    }
}

TEMPLATE_PRODUCT_TEST_CASE("{rows, cols,value} constructs a row-major buffer filled with data",
    "[Array2d][ctor]",
    (Array2d, Image, Kernel2d),
    (int,float,double)) {

    TestType val_{1,1};

    using T = std::remove_reference_t<decltype(val_(0, 0))>;

    std::size_t rows = 3;
    std::size_t cols = 5;
    std::size_t size = rows*cols;

    std::vector<T> data(size);
    T count{9};
    for (std::size_t r = 0; r < rows; ++r) {
        for (std::size_t c = 0; c < cols; ++c) {
            count +=1;
            data[r*cols + c] = count;
        }
    }

    SECTION("data matches provided in constructor."){

        TestType img{rows,cols,data};

        for (std::size_t r = 0; r < img.rows(); ++r) {
            for (std::size_t c = 0; c < img.cols(); ++c) {
                REQUIRE(img(r,c) == data[r*cols + c]);
            }
        }

    }

    SECTION("Required invalid data shape throws."){
        REQUIRE_THROWS_AS(
            (TestType{2,cols,data}),
            std::invalid_argument
        );
    }

}