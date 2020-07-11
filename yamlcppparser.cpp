#include "yamlcppparser.h"

#include <iostream>

namespace yamlcpp_differential_parser
{
// ---------------------------------------------------------------------------------
// ------------------------------ YamlCppParserOutput ---------------------------------
// ---------------------------------------------------------------------------------

YamlCppParserOutput::YamlCppParserOutput(std::string* info)
{
    this->data = info;
}

YamlCppParserOutput::~YamlCppParserOutput()
{
    if (this->data != nullptr)
    {
        delete this->data;
    }
}

bool YamlCppParserOutput::equivalent(ParserOutput* compared_object)
{
    return *(std::string*)this->getData() == *(std::string*)compared_object->getData();
}

void* YamlCppParserOutput::getData()
{
    return static_cast<void*>(this->data);
}

// ---------------------------------------------------------------------------------
// ---------------------------------- YamlCppParser -----------------------------------
// ---------------------------------------------------------------------------------

std::string YamlCppParser::getName()
{
    return "yaml";
}


void* YamlCppParser::parse(uint8_t*input, size_t input_size)
{
    return nullptr;
}

differential_parser::ParserOutput* YamlCppParser::normalize
    (void* input)
{   
    std::string* output = new std::string("yaml!");

    differential_parser::ParserOutput* returnMe = new
        YamlCppParserOutput (output);
    
    return returnMe;
}
}