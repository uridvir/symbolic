#pragma once

#include "alias.h"
#include "expression.h"

namespace symbolic {

template <Expression E1, Expression E2>
struct symbolic_ratio : public symbolic_expression {
   public:
    const E1 lhs;
    const E2 rhs;
    constexpr symbolic_ratio(const E1& lhs, const E2& rhs) : lhs(lhs), rhs(rhs) {}
};
template <Expression E1, Expression E2>
symbolic_complex_t symbolic_evaluate(const symbolic_ratio<E1, E2>& expr, const symbolic_complex_t& x) {
    return symbolic_evaluate(expr.lhs, x) / symbolic_evaluate(expr.rhs, x);
}
template <class T1, class T2>
constexpr symbolic_ratio<expression_type_t<T1>, expression_type_t<T2>> operator/(const T1& lhs, const T2& rhs) {
    return symbolic_ratio(expression_type_t<T1>(lhs), expression_type_t<T2>(rhs));
}

}