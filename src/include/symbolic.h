namespace symbolic {

class Symbol {};

enum FunctionType {
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

template<int N>
class Function {
    // private:
    // const FunctionType ft;
    // const Function& lhs;
    // const float C;
    // const Function& rhs;

    // constexpr Function(const FunctionType ft, const Function& lhs, const float C, const Function& rhs)
    //     : ft(ft), lhs(lhs), C(C), rhs(rhs) {};

    // public:
    // constexpr float operator()(const float& x) const {
    //     switch(ft){
    //         case Identity: return x;
    //         case Constant: return C;
    //         case Sum: return lhs(x) + rhs(x);
    //         default: return -9000;
    //     }
    // };
    // constexpr Function(const float x) : Function(Constant, *this, x, *this) {}
    // constexpr Function(const Symbol& s) : Function(Identity, *this, 0, *this) {}

    private:
    FunctionNode nodes[N];

    template<int N1, int N2, int N = N1 + N2 + 1>
    constexpr Function(FunctionType ft, const Function<N1>* lhs, const Function<N2>* rhs) : nodes() {
        nodes[0].ft = ft;
        
        auto moveNodes = [this](int start, int len, FunctionNode nodes2[]) {
            for (int i = 0; i < len; i++){
                nodes[i + start].ft = nodes2[i].ft;
                if (nodes[i + start].has_children = nodes2[i].has_children) {
                    nodes[i + start].children[0] = nodes2[i].children[0] + start;
                    nodes[i + start].children[1] = nodes2[i].children[1] + start;
                }
            }
        };

        nodes[0].children[0] = 1;
        moveNodes(1, N1, lhs->nodes);
        nodes[0].children[1] = 1 + N1;
        moveNodes(1 + N1, N2, rhs->nodes);
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
                std::cout << "Bad ft! " << nodes[i].ft << std::endl;
                return -9000;
        }
    }

    public:
    constexpr float operator()(const float x) const { return evaluateNode(0, x); }

    template<int N = 1>
    constexpr Function(const Symbol& sym) : nodes() {
        nodes[0].ft = Identity;
        nodes[0].sym = &sym;
        nodes[0].has_children = false;
    }

    template<int N = 1>
    constexpr Function(const float x) : nodes() {
        nodes[0].ft = Constant;
        nodes[0].C = x;
        nodes[0].has_children = false;
    }
    
    // template<class T1, class T2>
    // friend constexpr auto operator+(const T1& t1, const T2& t2);
};

// template<class T1, class T2>
// constexpr auto operator+(const T1& t1, const T2& t2){
//     auto f1 = t1;
//     auto f2 = t2;
//     return Function(Sum, f1, f2);
// }

}