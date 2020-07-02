#include "xmlparser.h"
#include <string>

#include <iostream>

std::string xaml_differential_parser::XmlParser::getName()
{
    return "xml";
}

void* xaml_differential_parser::XmlParser::parse(uint8_t* input, size_t input_size)
{
    return NULL;
}

void* xaml_differential_parser::XmlParser::normalize(void* input)
{
    std::string * returnMe = new std::string("Hello from xml!");
    
    return static_cast<void*>(returnMe);
}

bool xaml_differential_parser::XmlParser::equivalent(void* thing_one, void* thing_two)
{
    return *(std::string *)(thing_one) == *(std::string *)(thing_two);
}
