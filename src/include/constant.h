#pragma once

#include "alias.h"
#include "expression.h"

namespace symbolic {

template <Algebraic T>
struct symbolic_constant : symbolic_expression {
   public:
    const T C;
    constexpr symbolic_constant(const T& C) : C(C) {}
};
template <Algebraic T>
symbolic_complex_t symbolic_evaluate(const symbolic_constant<T>& expr, const symbolic_complex_t& x) {
    return expr.C;
}
template <Algebraic T>
struct expression_type<T> : std::type_identity<symbolic_constant<T>> {};

}