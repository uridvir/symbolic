#pragma once

#include "alias.h"
#include "expression.h"

namespace symbolic {

template <Expression E>
class Function {
   private:
    const E expr;

   public:
    constexpr Function(const E& expr) : expr(expr) {}

    //Simple functions
    template <class T>
    constexpr Function(const T& t) : Function(expression_type_t<T>(t)) {}

    //Evaluate
    symbolic_complex_t operator()(const symbolic_complex_t& x) const { return symbolic_evaluate(expr, x); }
};

template <class T>
Function(T) -> Function<expression_type_t<T>>;

}