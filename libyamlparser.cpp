#include "libyamlparser.h"

#include <iostream>

namespace libyaml_differential_parser
{
// ---------------------------------------------------------------------------------
// ------------------------------ LibyamlParserOutput ---------------------------------
// ---------------------------------------------------------------------------------

LibyamlParserOutput::LibyamlParserOutput(std::string* info)
{
    this->data = info;
}

LibyamlParserOutput::~LibyamlParserOutput()
{
    if (this->data != nullptr)
    {
        delete this->data;
    }
}

bool LibyamlParserOutput::equivalent(ParserOutput* compared_object)
{
    return *(std::string*)this->getData() == *(std::string*)compared_object->getData();
}

void* LibyamlParserOutput::getData()
{
    return static_cast<void*>(this->data);
}

// ---------------------------------------------------------------------------------
// ---------------------------------- LibyamlParser -----------------------------------
// ---------------------------------------------------------------------------------

std::string LibyamlParser::getName()
{
    return "yaml";
}


void* LibyamlParser::parse(uint8_t*input, size_t input_size)
{
    return nullptr;
}

differential_parser::ParserOutput* LibyamlParser::normalize
    (void* input)
{   
    std::string* output = new std::string("yaml!");

    differential_parser::ParserOutput* returnMe = new
        LibyamlParserOutput (output);
    
    return returnMe;
}
}