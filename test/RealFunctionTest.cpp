#include "symbolic.h"
#include <iostream>

using namespace symbolic;

int main(int argc, char* argv[]){

    Symbol x;

    Function f = x;
    Function g = f + f;

    std::cout << "g(x) = x + x; g(4) = " << g(4) << std::endl;

}