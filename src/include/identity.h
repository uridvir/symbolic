#pragma once

#include "alias.h"
#include "expression.h"

namespace symbolic {

struct symbolic_identity : symbolic_expression {
   public:
    const Symbol sym;
    constexpr symbolic_identity(const Symbol& sym) : sym(sym) {}
};
template <>
symbolic_complex_t symbolic_evaluate(const symbolic_identity& expr, const symbolic_complex_t& x) {
    return x;
}
template <>
struct expression_type<Symbol> : std::type_identity<symbolic_identity> {};

}