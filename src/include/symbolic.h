#include <cmath>
#include <complex>
#include <concepts>
#include <limits>

namespace symbolic {

class Symbol {
    public:
    const char* name;
    constexpr Symbol(const char* name) : name(name) {}
};

using symbolic_fundamental_t = long double;
using symbolic_complex_t = std::complex<symbolic_fundamental_t>;

template <typename T>
concept Algebraic = std::convertible_to<T, symbolic_complex_t>;

// Expression (base)
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

// Constant
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

//Identity
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

//Sum
template <Expression E1, Expression E2>
struct symbolic_sum : public symbolic_expression {
   public:
    const E1 lhs;
    const E2 rhs;
    constexpr symbolic_sum(const E1& lhs, const E2& rhs) : lhs(lhs), rhs(rhs) {}
};
template <Expression E1, Expression E2>
symbolic_complex_t symbolic_evaluate(const symbolic_sum<E1, E2>& expr, const symbolic_complex_t& x) {
    return symbolic_evaluate(expr.lhs, x) + symbolic_evaluate(expr.rhs, x);
}
template <class T1, class T2>
constexpr symbolic_sum<expression_type_t<T1>, expression_type_t<T2>> operator+(const T1& lhs, const T2& rhs) {
    return symbolic_sum(expression_type_t<T1>(lhs), expression_type_t<T2>(rhs));
}

//Difference
template <Expression E1, Expression E2>
struct symbolic_difference : public symbolic_expression {
   public:
    const E1 lhs;
    const E2 rhs;
    constexpr symbolic_difference(const E1& lhs, const E2& rhs) : lhs(lhs), rhs(rhs) {}
};
template <Expression E1, Expression E2>
symbolic_complex_t symbolic_evaluate(const symbolic_difference<E1, E2>& expr, const symbolic_complex_t& x) {
    return symbolic_evaluate(expr.lhs, x) - symbolic_evaluate(expr.rhs, x);
}
template <class T1, class T2>
constexpr symbolic_difference<expression_type_t<T1>, expression_type_t<T2>> operator-(const T1& lhs, const T2& rhs) {
    return symbolic_difference(expression_type_t<T1>(lhs), expression_type_t<T2>(rhs));
}

//Product
template <Expression E1, Expression E2>
struct symbolic_product : public symbolic_expression {
   public:
    const E1 lhs;
    const E2 rhs;
    constexpr symbolic_product(const E1& lhs, const E2& rhs) : lhs(lhs), rhs(rhs) {}
};
template <Expression E1, Expression E2>
symbolic_complex_t symbolic_evaluate(const symbolic_product<E1, E2>& expr, const symbolic_complex_t& x) {
    return symbolic_evaluate(expr.lhs, x) * symbolic_evaluate(expr.rhs, x);
}
template <class T1, class T2>
constexpr symbolic_product<expression_type_t<T1>, expression_type_t<T2>> operator*(const T1& lhs, const T2& rhs) {
    return symbolic_product(expression_type_t<T1>(lhs), expression_type_t<T2>(rhs));
}

//Ratio
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

//Power
template <Expression E1, Expression E2>
struct symbolic_power : public symbolic_expression {
   public:
    const E1 lhs;
    const E2 rhs;
    constexpr symbolic_power(const E1& lhs, const E2& rhs) : lhs(lhs), rhs(rhs) {}
};
template <Expression E1, Expression E2>
symbolic_complex_t symbolic_evaluate(const symbolic_power<E1, E2>& expr, const symbolic_complex_t& x) {
    return std::pow(symbolic_evaluate(expr.lhs, x), symbolic_evaluate(expr.rhs, x));
}
template <class T1, class T2>
constexpr symbolic_power<expression_type_t<T1>, expression_type_t<T2>> operator^(const T1& lhs, const T2& rhs) {
    return symbolic_power(expression_type_t<T1>(lhs), expression_type_t<T2>(rhs));
}

//Sine
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

}  // namespace symbolic