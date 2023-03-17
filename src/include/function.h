#pragma once

#include "alias.h"
#include "expression.h"
#include "constant.h"
#include "identity.h"

namespace symbolic {

template <Expression E>
class Function {
   private:
    const E expr;

   public:
    constexpr Function(const E& expr) : expr(expr) {}

    //Simple functions
    template <Algebraic T>
    constexpr Function(const T& t) : Function(symbolic_constant<T>(t)) {}
    constexpr Function(const Symbol& sym) : Function(symbolic_identity(sym)) {}

    //Evaluate
    symbolic_complex_t operator()(const symbolic_complex_t& x) const { return symbolic_evaluate(expr, x); }
};

template <Algebraic T>
Function(T) -> Function<symbolic_constant<T>>;
Function(Symbol) -> Function<symbolic_identity>;

}