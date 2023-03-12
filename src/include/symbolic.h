#pragma once

namespace symbolic {

class Function {
    public:
    virtual float operator()(float x) const { return 0; };
};

class IdentityFunction : public Function {
    public:
    IdentityFunction() {}
    float operator()(float x) const override { return x; }
};

class ConstantFunction : public Function {
    private:
    const float C;

    public:
    ConstantFunction(float C) : C(C) {};
    float operator()(float x) const override { return C; }
};

class SumFunction : public Function {
    private:
    const Function& lhs;
    const Function& rhs;

    public:
    SumFunction(const Function& lhs, const Function& rhs) : lhs(lhs), rhs(rhs) {}
    float operator()(float x) const override { return lhs(x) + rhs(x); }
};

class Symbol {
    public:
    operator Function() const { return IdentityFunction(); };
};

// SumFunction& operator+(const Function& lhs, const float rhs){
//     return SumFunction(lhs, ConstantFunction(rhs));
// }

// SumFunction& operator+(const Symbol& lhs, const float rhs){
//     return IdentityFunction() + rhs;
// }

}