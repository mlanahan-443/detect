#pragma once

#include <cstddef>
#include <stdexcept>
#include <utility>
#include <vector>
#include <mdspan>
#include<type_traits>
#include<span>

namespace array2d {

using Extents2d = std::dextents<std::size_t, 2>;
template <class T> using View2d  = std::mdspan<T, Extents2d>;
template <class T> using CView2d = std::mdspan<const T, Extents2d>;

/// @brief Owning, row-major container for 2-D Array2d data.
/// @tparam T Pixel type.
template <typename T, typename  Tag = void>
class Array2d {
    public:
        //default empty constructor
        Array2d() = default;
        //Constructor allowing Array2d(rows,cols,value)
        explicit Array2d(std::size_t rows, std::size_t cols, T fill = T{});
        //Constructor allowing Array2d(rows,cols,data)
        Array2d(std::size_t rows, std::size_t cols, std::vector<T> data);

        //Accessor allowing img[i,j] = value
        [[nodiscard]] T& operator()(std::size_t i, std::size_t j) noexcept { return data_[i * cols_ + j]; }
        //Accesor allowing value = img[i,j]
        [[nodiscard]] const T& operator()(std::size_t i, std::size_t j) const noexcept {
            return data_[i * cols_ + j];
        }

        //view functions for compatability with mdspan.
        [[nodiscard]] View2d<T> view() noexcept { return View2d(data_.data(), rows_, cols_); }
        [[nodiscard]] CView2d<T> view() const noexcept { return CView2d(data_.data(), rows_, cols_);}

        //Get rows
        [[nodiscard]] std::size_t rows() const noexcept { return rows_; }
        //Get columns
        [[nodiscard]] std::size_t cols() const noexcept { return cols_; }
        //Get size of Array2d
        [[nodiscard]] std::size_t size() const noexcept { return data_.size(); }
        //Get data of Array2d
        [[nodiscard]] std::span<const T> data() const noexcept { return data_; }    
        //Storage of private variables
    private:
        //Rows 
        std::size_t rows_{0};
        //Columns
        std::size_t cols_{0};
        //Array2d data
        std::vector<T> data_;
};

/*Constructor definitions*/

//Constructor that populates the rows,cols, and data with the fill value.
template <typename T, typename Tag>
Array2d<T, Tag>::Array2d(std::size_t rows, std::size_t cols, T fill)
    : rows_{rows}, cols_{cols}, data_(rows * cols, fill) {}


//Constructor that populates the rows,cols, and data with the supplied vector of data.
template <typename T, typename Tag>
Array2d<T, Tag>::Array2d(std::size_t rows, std::size_t cols, std::vector<T> data)
    : rows_{rows}, cols_{cols}, data_(std::move(data)) {
    //Check if the supplied data matches the size of the rows*cols
  if (data_.size() != rows_ * cols_) {
    throw std::invalid_argument("Array2d: data.size() must equal rows * cols");
  }
}


}  // namespace Array2d