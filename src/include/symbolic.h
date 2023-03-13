#include <cmath>
#include <limits>

namespace symbolic {

class Symbol {};

enum FunctionType {
    // Default
    Null,
    // Variables
    Identity,
    // Numbers
    Constant,
    // Operations
    Sum,
    Difference,
    Product,
    Ratio,
    Power,
    // Composition
    Composition,
    // Primitives
    Sine,
};

struct FunctionNode {
    FunctionType ft;
    union {
        int children[2];
        float C;
        const Symbol* sym;
    };
    bool has_children;
};

const std::size_t NODES_MAX = 1024;

class Function {
   private:
    const std::size_t N;
    FunctionNode nodes[NODES_MAX];

    constexpr void moveNodes(const std::size_t start, const std::size_t len, const FunctionNode nodes2[]) {
        for (std::size_t i = 0; i < len; i++) {
            nodes[i + start] = nodes2[i];
            if (nodes[i + start].has_children) {
                nodes[i + start].children[0] += start;
                nodes[i + start].children[1] += start;
            }
        }
    }

    constexpr Function(const FunctionType ft, const Function& lhs, const Function& rhs) : N(lhs.N + rhs.N + 1), nodes() {
        nodes[0].ft = ft;
        nodes[0].children[0] = 1;
        moveNodes(1, lhs.N, lhs.nodes);
        nodes[0].children[1] = 1 + lhs.N;
        moveNodes(1 + lhs.N, rhs.N, rhs.nodes);
        nodes[0].has_children = true;
    }

    constexpr Function(const FunctionType ft, const Function& lhs) : N(lhs.N + 1), nodes() {
        nodes[0].ft = ft;
        nodes[0].children[0] = 1;
        moveNodes(1, lhs.N, lhs.nodes);
        nodes[0].has_children = true;
    }

    // //Create a Function from part of the tree of another Function
    // constexpr Function(const std::size_t start, const std::size_t len, const FunctionNode nodes2[]) : N(len), nodes() {
    //     for (std::size_t i = 0; i < len; i++){
    //         nodes[i] = nodes2[i + start];
    //         if (nodes[i].has_children) {
    //             nodes[i].children[0] -= start;
    //             nodes[i].children[1] -= start;
    //         }
    //     }
    // }

    constexpr float evaluateNode(const std::size_t i, const float x) const {
        switch (nodes[i].ft) {
            case Identity:
                return x;
            case Constant:
                return nodes[i].C;
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
            // //Composition
            // case Composition:
            // {
            //     auto f = Function(nodes[i].children[0], nodes[i].children[1] - nodes[i].children[0], nodes);
            //     return f(evaluateNode(nodes[i].children[1], x));
            // }
            // Primitives
            case Sine:
                return std::sin(evaluateNode(nodes[i].children[0], x));
            default:
                return std::numeric_limits<float>::quiet_NaN();
        }
    }

   public:
    // Function evaluation
    constexpr float operator()(const float x) const { return evaluateNode(0, x); }

    // Direct conversions
    constexpr Function(const Symbol& sym) : N(1), nodes() {
        nodes[0].ft = Identity;
        nodes[0].sym = &sym;
        nodes[0].has_children = false;
    }
    constexpr Function(const float x) : N(1), nodes() {
        nodes[0].ft = Constant;
        nodes[0].C = x;
        nodes[0].has_children = false;
    }

    // Binary operations
    template <class T1, class T2>
    friend constexpr Function operator+(const T1& t1, const T2& t2);
    template <class T1, class T2>
    friend constexpr Function operator-(const T1& t1, const T2& t2);
    template <class T1, class T2>
    friend constexpr Function operator*(const T1& t1, const T2& t2);
    template <class T1, class T2>
    friend constexpr Function operator/(const T1& t1, const T2& t2);
    template <class T1, class T2>
    friend constexpr Function operator^(const T1& t1, const T2& t2);

    // //Composition
    // template<class T>
    // constexpr Function operator()(const T& t) {
    //     auto f = t;
    //     return Function(Composition, *this, f);
    // }

    // Primitive functions
    template <class T>
    friend constexpr Function sine(const T& f);
};

template <class T1, class T2>
constexpr Function operator+(const T1& t1, const T2& t2) {
    return Function(Sum, t1, t2);
}
template <class T1, class T2>
constexpr Function operator-(const T1& t1, const T2& t2) {
    return Function(Difference, t1, t2);
}
template <class T1, class T2>
constexpr Function operator*(const T1& t1, const T2& t2) {
    return Function(Product, t1, t2);
}
template <class T1, class T2>
constexpr Function operator/(const T1& t1, const T2& t2) {
    return Function(Ratio, t1, t2);
}
template <class T1, class T2>
constexpr Function operator^(const T1& t1, const T2& t2) {
    return Function(Power, t1, t2);
}
template <class T>
constexpr Function sine(const T& t) {
    return Function(Sine, t);
}

}  // namespace symbolic