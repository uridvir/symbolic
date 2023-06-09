#include "symbolic.h"
#include <iostream>

using namespace symbolic;

int main(int argc, char* argv[]){

    constexpr Symbol x("x");

    constexpr Function f = sine(((2 * x) ^ 2) + ((4 * x) ^ 2));

    std::cout << f(1) << std::endl;

}