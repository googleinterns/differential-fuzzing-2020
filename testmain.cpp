// Your First C++ Program

#include <iostream>
#include "xmlparser.h"
#include "yamlparser.h"
#include "parser.h"

#include "differentialfuzzer.cpp"

// Main used for the purpose of minor testing
int main() 
{
    XmlParser xmlcase;
    YamlParser yamlcase;
    std::cout << "Welcome to the test suite" << std::endl << std::endl;
    
    ParserContainer* testOne = new ParserContainer();
    
    return 0;
}