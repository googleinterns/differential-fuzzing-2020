#include <iostream>
#include "./../../libraries/libyaml/include/yaml.h"
#include "./../../libraries/yaml-cpp/include/yaml-cpp/anchor.h"

// #include "xmlparser.h"
// #include "yamlparser.h"
// #include "parser.h"

// #include "yaml-cpp/exceptions.h"

// #include "differentialfuzzer.h"

// Main used for the purpose of minor testing
int main() 
{
    std::cout << "Do thing!" << std::endl;
    // YAML::Node primes = YAML::Load("[2, 3, 5, 7, 11]");

    yaml_parser_t parser;
    yaml_event_t  event; 
    if(!yaml_parser_initialize(&parser))
    {
        std::cout << "Failed to initialize parser!" << std::endl;
    }
    else
    {
        std::cout << "Parser recognized!" << std::endl;
    }
    
    return 0;
}