// Your First C++ Program

#include <iostream>
#include "xmlparser.h"
#include "yamlparser.h"
#include "parser.h"

#include "fuzzer.cpp"
// #include "../libraries/yaml-cpp/build/libyaml-cpp.a"

// Main used for the purpose of minor testing
int main() 
{
    XmlParser xmlcase;
    YamlParser yamlcase;
    std::cout << "Welcome to the test suite" << std::endl << std::endl;
    
    ParserContainer* testOne = new ParserContainer();
    // testOne->data = (Parser*)(&xmlcase);

    // int t = LLVMFuzzerTestOneInput(NULL, 1);
    return 0;
}