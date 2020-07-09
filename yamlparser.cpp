#include "yamlparser.h"

#include <iostream>

namespace yaml_differential_parser
{
// ---------------------------------------------------------------------------------
// ------------------------------ YamlParserOutput ---------------------------------
// ---------------------------------------------------------------------------------

YamlParserOutput::YamlParserOutput(std::string* info)
{
    this->data = info;
}

YamlParserOutput::~YamlParserOutput()
{
    if (this->data != nullptr)
    {
        delete this->data;
    }
}

bool YamlParserOutput::equivalent(ParserOutput* compared_object)
{
    return *(std::string*)this->getData() == *(std::string*)compared_object->getData();
}

void* YamlParserOutput::getData()
{
    return static_cast<void*>(this->data);
}

// ---------------------------------------------------------------------------------
// ---------------------------------- YamlParser -----------------------------------
// ---------------------------------------------------------------------------------

std::string YamlParser::getName()
{
    return "yaml";
}


void* YamlParser::parse(uint8_t*input, size_t input_size)
{
    return nullptr;
}

differential_parser::ParserOutput* YamlParser::normalize
    (void* input)
{   
    std::string* output = new std::string("yaml!");

    differential_parser::ParserOutput* returnMe = new
        YamlParserOutput (output);
    
    return returnMe;
}
}