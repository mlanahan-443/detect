#pragma once

#include <mdspan>
#include <format>
#include <algorithm>

#include "array2d.hpp"
#include "kernel2d.hpp"

namespace array2d {
/**
 * @brief Convolves two 2d mdspan objects using "same size" methodology.
 * 
 * Only the first H,W rows and colummns will be filled in "out".
 * @param in The input 2d array-like object to convolve.
 * @param kernel The kernel to convolve with the input.
 * @param out The output object container.
 * @throws std::invalid_argument if the number of rows in the output image is less than the number of rows in the input.
 */
template <typename  Tin, typename  Tkernel, typename  Tout>
void convolve2d(array2d::CView2d<Tin> in,
                array2d::CView2d<Tkernel> kernel,
                array2d::View2d<Tout> out)
{
                
    
    using idx = std::ptrdiff_t;  // signed: offsets go negative

    //Get input rows/cols 
    const idx H  = static_cast<idx>(in.extent(0));
    const idx W  = static_cast<idx>(in.extent(1));


    //Check valid shape of input/output.
    if (out.extents() != in.extents()) {
        throw std::invalid_argument(std::format(
            "convolve2d: output shape ({}, {}) must match input shape ({}, {})",
            out.extent(0), out.extent(1), in.extent(0), in.extent(1)));
    }

    //Half-widths of 2d kernel
    const idx a = static_cast<idx>(kernel2d::detail::require_odd(kernel.extent(0)) / 2);
    const idx b = static_cast<idx>(kernel2d::detail::require_odd(kernel.extent(1)) / 2);

    //Early exit for trivial calculation
    if (H <= 0 || W <=0 || a <= 0 || b <= 0){
        return;
    }

    for (idx m = 0; m < H; ++m) {
        //Get allowable row bounds of convolution kernel 
        const idx i_lo = std::max(-a, m - H + 1);
        const idx i_hi =  std::min( a, m);

        for (idx n = 0; n < W; ++n) {
            //Get allowable column bounds of convolution kernel
            const idx j_lo = std::max(-b, n - W + 1);
            const idx j_hi = std::min( b, n);
            
            // initialize accumulation variable to 0
            Tout acc{};
            
            // Loop over in-bounds extent of kernel.
            for (idx i = i_lo; i <= i_hi; ++i)
                for (idx j = j_lo; j <= j_hi; ++j)
                    acc += static_cast<Tout>(kernel[i + a, j + b])
                         * static_cast<Tout>(in[m - i, n - j]);
            
            //Assign to output.
            out[m, n] = acc;
        };
    };
}

template <typename Tin, typename Tk, typename Tout, typename TagIn, typename TagK, typename TagOut>
void convolve2d(const array2d::Array2d<Tin, TagIn>& in,
                const array2d::Array2d<Tk,  TagK>&  kernel,
                array2d::Array2d<Tout, TagOut>&     out)
{
    convolve2d<Tin, Tk, Tout>(in.view(), kernel.view(), out.view());
}

};