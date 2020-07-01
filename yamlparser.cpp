#include "yamlparser.h"
#include <string>

#include <iostream>

std::string YamlParser::getName()
{
    return "yaml";
}


void* YamlParser::parse(uint8_t*input, size_t input_size)
{
    return NULL;
}

void* YamlParser::normalize(void* input)
{
    std::string * returnMe = new std::string("Hello from yaml!");
    return static_cast<void*>(returnMe);
}

bool YamlParser::equivalent(void* thing_one, void* thing_two)
{
    // std::cout << "yaml equivalent" << std::endl;
    // std::cout << "thing one-> "<< *(std::string *)(thing_one) << std::endl;
    // std::cout << "thing two-> "<< *(std::string *)(thing_two) << std::endl;
    return *(std::string *)(thing_one) == *(std::string *)(thing_two);
}
