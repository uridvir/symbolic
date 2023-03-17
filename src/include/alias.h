#pragma once

#include <complex>
#include <concepts>

namespace symbolic {

using symbolic_fundamental_t = long double;
using symbolic_complex_t = std::complex<symbolic_fundamental_t>;
template <typename T>
concept Algebraic = std::convertible_to<T, symbolic_complex_t>;

}