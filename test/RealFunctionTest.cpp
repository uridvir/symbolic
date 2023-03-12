#include "symbolic.h"
#include <iostream>

using namespace symbolic;

int main(int argc, char* argv[]){

    Symbol x;

    Function f = x;
    Function g = 2;
    Function h = f + x;

    std::cout << h(4) << std::endl;

}