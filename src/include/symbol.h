namespace symbolic {

class Symbol {
    public:
    const char* name;
    constexpr Symbol(const char* name) : name(name) {}
};

}