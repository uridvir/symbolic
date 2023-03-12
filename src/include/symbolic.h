#include <limits>

namespace symbolic {

class Symbol {};

enum FunctionType {
    Null,
    Identity,
    Constant,
    Sum,
    // Difference,
    // Product,
    // Ratio,
};

struct FunctionNode {
    FunctionType ft;
    union
    {
        int children[2];
        float C;
        const Symbol* sym;
    };
    bool has_children;
};

const int NODES_MAX = 1024;

class Function {
    private:
    const int N;
    FunctionNode nodes[NODES_MAX];

    constexpr void moveNodes(const int start, const int len, const FunctionNode nodes2[]){
        for (int i = 0; i < len; i++){
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

    constexpr float evaluateNode(const int i, const float x) const {
        switch(nodes[i].ft){
            case Identity:
                return x;
            case Constant:
                return nodes[i].C;
            case Sum: 
                return evaluateNode(nodes[i].children[0], x) + evaluateNode(nodes[i].children[1], x);
            default:
                return std::numeric_limits<float>::quiet_NaN();
        }
    }

    public:
    constexpr float operator()(const float x) const { return evaluateNode(0, x); }

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
    
    template<class T1, class T2>
    friend constexpr Function operator+(const T1& t1, const T2& t2);
};

template<class T1, class T2>
constexpr Function operator+(const T1& t1, const T2& t2){
    auto f1 = t1;
    auto f2 = t2;
    return Function(Sum, f1, f2);
}

}