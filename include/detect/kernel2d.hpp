#pragma once

#include <cstddef>
#include <stdexcept>
#include "array2d.hpp"

namespace kernel2d {

namespace detail {

    ///@brief Checks if the argument is odd.
    ///@throw std::invalid_argument If the dimension is not odd.
    constexpr std::size_t require_odd(std::size_t n) {
        if (n % 2 == 0) {
            throw std::invalid_argument("Kernel2d: dimensions must be odd");
        }
        return n;
    }
    
    }  // namespace detail

/// @brief Owning, row-major container for 2-D Kernel.
/// @tparam T Pixel type.
template <typename T>
class Kernel2d: public array2d::Array2d<T>{ 
    using Base = array2d::Array2d<T>;

    public:
        Kernel2d() = delete;
        explicit Kernel2d(std::size_t rows, std::size_t cols, T fill = T{});
        Kernel2d(std::size_t rows, std::size_t cols, std::vector<T> data);
};

template <typename T>
Kernel2d<T>::Kernel2d(std::size_t rows, std::size_t cols, T fill)
    : Base{detail::require_odd(rows), detail::require_odd(cols), fill}
{}

template <typename T>
Kernel2d<T>::Kernel2d(std::size_t rows, std::size_t cols, std::vector<T> data)
    : Base{detail::require_odd(rows), detail::require_odd(cols), std::move(data)}
{}

};