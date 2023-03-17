#pragma once

#include "alias.h"
#include "expression.h"

namespace symbolic {

template <Expression E>
struct symbolic_sine : public symbolic_expression {
   public:
    const E expr;
    constexpr symbolic_sine(const E& expr) : expr(expr) {}
};
template <Expression E>
symbolic_complex_t symbolic_evaluate(const symbolic_sine<E>& expr, const symbolic_complex_t& x) {
    return std::sin(symbolic_evaluate(expr.expr, x));
}
template <class T>
constexpr symbolic_sine<expression_type_t<T>> sine(const T& t) {
    return symbolic_sine(expression_type_t<T>(t));
}

}