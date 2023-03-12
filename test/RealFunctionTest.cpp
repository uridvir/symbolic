#include "symbolic.h"
#include <iostream>

using namespace symbolic;

int main(int argc, char* argv[]){

    Symbol x;

    Function& f = IdentityFunction();

    std::cout << "f(x) = x; f(4) = " << f(4) << std::endl;

}