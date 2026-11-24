// tests/test_Array2d.cpp
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>

#include "detect/convolve.hpp"
#include "detect/array2d.hpp"
#include "detect/image.hpp"
#include "detect/kernel2d.hpp"


using array2d::Array2d;
using kernel2d::Kernel2d;

TEMPLATE_PRODUCT_TEST_CASE("Test basic trivial inputs",
                          "[convolve2d][input]",
                          (Array2d, Image),
                            (int,double,float)) {

    SECTION("Convolution with either zero-valued array or kernel (for all types of Array2d + dtypes is zero-valued"){

        //Allocate arguments.
        TestType array_zero{9, 9};
        TestType kernel_zero{3,3};

        TestType array_four{9,9,4};
        TestType kernel_four{3,3,4};
        
        //Allocate result container.

        for (const auto* array_in : {&array_zero, &array_four}) {
            for (const auto* kernel : {&kernel_zero, &kernel_four}) {
                if (array_in == &array_four && kernel == &kernel_four) continue;
                
                TestType result{9,9};
                array2d::convolve2d(*array_in,*kernel, result);
                using T = std::remove_reference_t<decltype(array_zero(0, 0))>;
                    for (std::size_t i = 0;i < result.rows(); i ++){
                        for (std::size_t j = 0; j < result.cols(); j ++){
                            REQUIRE(result(i,j) == T{});
                        };
                    };
            }
        }
       
    }

    SECTION("Convolution against snapshot handworked result."){
        //Handworked simple convolution result.
        // increment from 0 to 8 with a 3x3 image, convolve with a 3x3 kernel of all 1's.
        TestType arr_{1,1};
        using T = std::remove_reference_t<decltype(arr_(0, 0))>;

        Kernel2d<T> kernel_test_{1,1};

        std::vector<T> kernel_vec{1,1,1,1,1,0,1,0,0};
        Kernel2d kernel{3,3,kernel_vec};

        std::vector<T> eo{11, 10,  7,  4, 10,  3, 11, 11,15, 12, 14,  7 ,12,  3,  7,  0};
        Array2d expected_out{4,4,eo};

        std::vector<T> array_vec{1,2,0,0,5,3,0,4,0,0,0,7,9,3,0,0};

        TestType array_in{4,4,array_vec};
        TestType result{4,4};
        
        array2d::convolve2d(array_in,kernel,result);

        for (std::size_t i = 0;i < result.rows(); i ++){
            for (std::size_t j = 0; j < result.cols(); j ++){
                CAPTURE(i,j);
                CHECK(result(i,j) == expected_out(i,j));
            }
        }
    }

}
