#include "symbolic.h"
#include <iostream>

using namespace symbolic;

int main(int argc, char* argv[]){

    Symbol x;

    Function f = sine((2 * (x ^ 2))/3 - 1);

    std::cout << f(4) << std::endl;

}