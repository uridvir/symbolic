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

class Function {
    private:
    const FunctionType ft;
    const Function& lhs;
    const float C;
    const Function& rhs;

    constexpr Function(const FunctionType ft, const Function& lhs, const float C, const Function& rhs)
        : ft(ft), lhs(lhs), C(C), rhs(rhs) {};

    public:
    constexpr float operator()(const float& x) const {
        switch(ft){
            case Identity: return x;
            case Constant: return C;
            case Sum: return lhs(x) + rhs(x);
            default: return -9000;
        }
    };
    constexpr Function(const float x) : Function(Constant, *this, x, *this) {}
    constexpr Function(const Symbol& s) : Function(Identity, *this, 0, *this) {}
    
    // constexpr Function operator+(const float& x) { //TODO: Addition operators are letting the new children die, FIX THIS
    //     Function f = x;
    //     return *this + f;
    // }
    // constexpr Function operator+(const Symbol& s) { //TODO: ...
    //     Function f = s;
    //     return *this + f;
    // }
    // constexpr Function operator+(const Function& f) {
    //     return Function(Sum, *this, 0, f);
    // }

    // template<class T1, class T2>
    // friend constexpr Function operator+(const T1& t1, const T2& t2){
    //     Function f1 = t1;
    //     Function f2 = t2;
    //     return Function(Sum, f1, 0, f2);
    // }
    template<class T1, class T2>
    friend constexpr Function operator+(const T1& t1, const T2& t2);
};

template<class T1, class T2>
constexpr Function operator+(const T1& t1, const T2& t2){
    Function f1 = t1;
    Function f2 = t2;
    return Function(Sum, f1, 0, f2);
}

}