#include "symbolic.h"
#include <iostream>

using namespace symbolic;

int main(int argc, char* argv[]){

    Symbol x;

    Function<2> f = x;
    Function<2> g = 2;
    // Function<3> h = x + 2;

    std::cout << f(4) << std::endl;

}