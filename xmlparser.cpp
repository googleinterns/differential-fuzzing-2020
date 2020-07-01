#include "xmlparser.h"
#include <string>

#include <iostream>

std::string XmlParser::getName()
{
    return "xml";
}

void* XmlParser::parse(uint8_t* input, size_t input_size)
{
    return NULL;
}

void* XmlParser::normalize(void* input)
{
    std::string * returnMe = new std::string("Hello from xml!");
    return static_cast<void*>(returnMe);
}

bool XmlParser::equivalent(void* thing_one, void* thing_two)
{
    return *(std::string *)(thing_one) == *(std::string *)(thing_two);
}
