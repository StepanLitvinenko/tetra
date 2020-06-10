#pragma once
#include <cstdint>
#include "complex.h"
using ShortComplex = Complex<std::int16_t>;
using WordComplex = Complex<std::int32_t>;
using FloatComplex = Complex<float>;
template <typename T> using AnyComplex = Complex<T>;
