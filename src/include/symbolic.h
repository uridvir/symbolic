#include <cmath>
#include <complex>
#include <concepts>
#include <limits>

namespace symbolic {

class Symbol {};

enum FunctionType {
    Null,      // Default
    Identity,  // Variables
    Constant,  // Numbers
    Sum,
    Difference,
    Product,
    Ratio,
    Power,        // Operations
    Composition,  // Composition
    Sine,         // Primitives
};

using symbolic_fundamental_t = long double;
using symbolic_complex_t = std::complex<symbolic_fundamental_t>;

struct FunctionNode {
    FunctionType ft;
    union {
        int children[2];
        symbolic_fundamental_t C[2];
        const Symbol* sym;
    };
    bool has_children;
};

/**
 * Move n nodes from nodes1, starting at index start1, to nodes2, starting at index start2.
 * Handles transformation of indices.
 */
void moveNodes(std::size_t n, const FunctionNode nodes1[], std::size_t start1, FunctionNode nodes2[], std::size_t start2) {
    for (std::size_t i = 0; i < n; i++) {
        nodes2[i + start2] = nodes1[i + start1];
        if (nodes2[i + start2].has_children) {
            nodes2[i + start2].children[0] += start2 - start1;
            nodes2[i + start2].children[1] += start2 - start1;
        }
    }
}

template <int N>
class Function_rvalue;

template <typename T>
struct node_count : std::integral_constant<int, 0> {};
template <typename T>
constexpr int node_count_v = node_count<T>::value;
template <>
struct node_count<Symbol> : std::integral_constant<int, 1> {};
template <typename T>
concept Algebraic = std::convertible_to<T, symbolic_complex_t>;
template <Algebraic T>
struct node_count<T> : std::integral_constant<int, 1> {};
template <int N>
struct node_count<Function_rvalue<N>> : std::integral_constant<int, N> {};

template <int N>
class Function_rvalue {
   private:
    FunctionNode nodes[N];

    template <int N1>
    friend class Function_rvalue;

    template <Algebraic InputType, Algebraic OutputType>
    friend class Function_lvalue;

    template <int N1, int N2, typename = std::enable_if_t<N == N1 + N2 + 1>>
    constexpr Function_rvalue(FunctionType ft, const Function_rvalue<N1>& lhs, const Function_rvalue<N2>& rhs) {
        nodes[0].ft = ft;
        nodes[0].has_children = true;
        nodes[0].children[0] = 1;
        nodes[0].children[1] = 1 + N1;
        moveNodes(N1, lhs.nodes, 0, nodes, nodes[0].children[0]);
        moveNodes(N2, rhs.nodes, 0, nodes, nodes[0].children[1]);
    }

    template <int N1, typename = std::enable_if_t<N == N1 + 1>>
    constexpr Function_rvalue(FunctionType ft, const Function_rvalue<N1>& lhs) {
        nodes[0].ft = ft;
        nodes[0].has_children = true;
        nodes[0].children[0] = 1;
        moveNodes(N1, lhs.nodes, 0, nodes, nodes[0].children[0]);
    }

   public:
   /**
    * TODO: Fix design flaw that causes GCC to give template errors for N > 1.
    * For N > 1, every specialization of these constructors is ill-formed. Therefore,
    * the compiler is within its rights to disallow this.
    * See https://developercommunity.visualstudio.com/t/enable-if-powered-sfinae-fails-to-compile-even-on/556453
   */
    template <typename = std::enable_if_t<N == 1>>
    constexpr Function_rvalue(const Symbol& sym) {
        nodes[0].ft = Identity;
        nodes[0].sym = &sym;
        nodes[0].has_children = false;
    }
    template <Algebraic T, typename = std::enable_if_t<N == 1>>
    constexpr Function_rvalue(const T& t) {
        nodes[0].ft = Constant;
        std::complex c = t;
        nodes[0].C[0] = c.real();
        nodes[0].C[1] = c.imag();
        nodes[0].has_children = false;
    }

    // Binary operations
    template <class T1, class T2>
    friend constexpr Function_rvalue<node_count_v<T1> + node_count_v<T2> + 1> operator+(const T1& t1, const T2& t2);
    template <class T1, class T2>
    friend constexpr Function_rvalue<node_count_v<T1> + node_count_v<T2> + 1> operator-(const T1& t1, const T2& t2);
    template <class T1, class T2>
    friend constexpr Function_rvalue<node_count_v<T1> + node_count_v<T2> + 1> operator*(const T1& t1, const T2& t2);
    template <class T1, class T2>
    friend constexpr Function_rvalue<node_count_v<T1> + node_count_v<T2> + 1> operator/(const T1& t1, const T2& t2);
    template <class T1, class T2>
    friend constexpr Function_rvalue<node_count_v<T1> + node_count_v<T2> + 1> operator^(const T1& t1, const T2& t2);

    // Primitive functions
    template <class T>
    friend constexpr Function_rvalue<node_count_v<T> + 1> sine(const T& t);
    // template <Simple T>
    // friend constexpr Function_rvalue<2> sine(const T& t);
};

template <class T1, class T2>
constexpr Function_rvalue<node_count_v<T1> + node_count_v<T2> + 1> operator+(const T1& t1, const T2& t2) {
    const int N1 = node_count_v<T1>; const int N2 = node_count_v<T2>; const int N = N1 + N2 + 1;
    return Function_rvalue<N>(Sum, Function_rvalue<N1>(t1), Function_rvalue<N2>(t2));
}
template <class T1, class T2>
constexpr Function_rvalue<node_count_v<T1> + node_count_v<T2> + 1> operator-(const T1& t1, const T2& t2) {
    const int N1 = node_count_v<T1>; const int N2 = node_count_v<T2>; const int N = N1 + N2 + 1;
    return Function_rvalue<N>(Difference, Function_rvalue<N1>(t1), Function_rvalue<N2>(t2));
}
template <class T1, class T2>
constexpr Function_rvalue<node_count_v<T1> + node_count_v<T2> + 1> operator*(const T1& t1, const T2& t2) {
    const int N1 = node_count_v<T1>; const int N2 = node_count_v<T2>; const int N = N1 + N2 + 1;
    return Function_rvalue<N>(Product, Function_rvalue<N1>(t1), Function_rvalue<N2>(t2));
}
template <class T1, class T2>
constexpr Function_rvalue<node_count_v<T1> + node_count_v<T2> + 1> operator/(const T1& t1, const T2& t2) {
    const int N1 = node_count_v<T1>; const int N2 = node_count_v<T2>; const int N = N1 + N2 + 1;
    return Function_rvalue<N>(Ratio, Function_rvalue<N1>(t1), Function_rvalue<N2>(t2));
}
template <class T1, class T2>
constexpr Function_rvalue<node_count_v<T1> + node_count_v<T2> + 1> operator^(const T1& t1, const T2& t2) {
    const int N1 = node_count_v<T1>; const int N2 = node_count_v<T2>; const int N = N1 + N2 + 1;
    return Function_rvalue<N>(Power, Function_rvalue<N1>(t1), Function_rvalue<N2>(t2));
}
template <class T>
constexpr Function_rvalue<node_count_v<T> + 1> sine(const T& t) {
    const int N1 = node_count_v<T>; const int N = N1 + 1;
    return Function_rvalue<N>(Sine, Function_rvalue<N1>(t));
}

const int MAX_NODES = 1024;

template <Algebraic InputType, Algebraic OutputType>
class Function_lvalue {
    private:
    FunctionNode nodes[MAX_NODES];
    const int N;

    constexpr OutputType evaluateNode(const std::size_t i, const InputType& x) const {
        switch (nodes[i].ft) {
            case Identity:
                return x;
            case Constant:
                return nodes[i].C[0]; //TODO: Proper down-casting from complex
            // Binary operations
            case Sum:
                return evaluateNode(nodes[i].children[0], x) + evaluateNode(nodes[i].children[1], x);
            case Difference:
                return evaluateNode(nodes[i].children[0], x) - evaluateNode(nodes[i].children[1], x);
            case Product:
                return evaluateNode(nodes[i].children[0], x) * evaluateNode(nodes[i].children[1], x);
            case Ratio:
                return evaluateNode(nodes[i].children[0], x) / evaluateNode(nodes[i].children[1], x);
            case Power:
                return std::pow(evaluateNode(nodes[i].children[0], x), evaluateNode(nodes[i].children[1], x));
            // Primitives
            case Sine:
                return std::sin(evaluateNode(nodes[i].children[0], x));
            default:
                return std::numeric_limits<float>::quiet_NaN();
        }
    }

    public:
    template <int N1>
    constexpr Function_lvalue(const Function_rvalue<N1>& rhs) : nodes(), N(N1) {
        for (std::size_t i = 0; i < N; i++) nodes[i] = rhs.nodes[i];
    }

    template <class T>
    constexpr Function_lvalue(const T& t) : Function_lvalue(Function_rvalue<node_count_v<T>>(t)) {}

    constexpr OutputType operator()(const InputType& x) const {
        return evaluateNode(0, x);
    }    
};

template <Algebraic InputType, Algebraic OutputType>
using Function = Function_lvalue<InputType, OutputType>;

}  // namespace symbolic