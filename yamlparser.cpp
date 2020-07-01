#include "yamlparser.h"
#include <string>

#include <iostream>

std::string yaml_differential_parser::YamlParser::getName()
{
    return "yaml";
}


void* yaml_differential_parser::YamlParser::parse(uint8_t*input, size_t input_size)
{
    return NULL;
}

void* yaml_differential_parser::YamlParser::normalize(void* input)
{
    std::string * returnMe = new std::string("Hello from yaml!");
    return static_cast<void*>(returnMe);
}

bool yaml_differential_parser::YamlParser::equivalent(void* thing_one, void* thing_two)
{
    // std::cout << "yaml equivalent" << std::endl;
    // std::cout << "thing one-> "<< *(std::string *)(thing_one) << std::endl;
    // std::cout << "thing two-> "<< *(std::string *)(thing_two) << std::endl;
    return *(std::string *)(thing_one) == *(std::string *)(thing_two);
}
