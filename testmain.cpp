// Your First C++ Program

#include <iostream>
#include "xmlparser.h"
#include "yamlparser.h"
#include "parser.h"
// #include "../libraries/yaml-cpp/build/libyaml-cpp.a"

// Main used for the purpose of minor testing
int main() 
{
    
    std::cout << "Welcome to the test suit" << std::endl << std::endl;
    yamlparser funparser;
    std::cout << funparser.normalize(0);
    return 0;
}