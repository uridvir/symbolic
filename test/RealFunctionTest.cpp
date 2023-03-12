#include "symbolic.h"
#include <iostream>

using namespace symbolic;

int main(int argc, char* argv[]){

    Symbol x;

    Function f = x + x + 2;

    std::cout << f(4) << std::endl;

}