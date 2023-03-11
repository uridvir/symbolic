#include "symbolic.h"
#include <iostream>

using namespace symbolic;

int main(int argc, char* argv[]){

    Symbol x;

    Function f = x + 2;

    std::cout << "f(x) = x + 2; f(4) = " << f(4) << std::endl;

}