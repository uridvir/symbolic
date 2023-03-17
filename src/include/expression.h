#pragma once

#include "alias.h"

#include <concepts>

namespace symbolic {

struct symbolic_expression {};
template <class T>
concept Expression = std::derived_from<T, symbolic_expression>;
template <Expression E>
symbolic_complex_t symbolic_evaluate(const E& expr, const symbolic_complex_t& x) {  // Base template
    return std::numeric_limits<symbolic_fundamental_t>::quiet_NaN();
}
template <class T>
struct expression_type : std::type_identity<symbolic_expression> {};
template <Expression E>
struct expression_type<E> : std::type_identity<E> {};
template <class T>
using expression_type_t = expression_type<T>::type;

}