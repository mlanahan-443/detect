#include "array2d.hpp"

//Image tag
struct ImageTag {};

//Image type
template <class T> using Image = array2d::Array2d<T,ImageTag>;