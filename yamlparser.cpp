#include "yamlparser.h"
#include <string>

#include <iostream>


// ---------------------------------------------------------------------------------
// ------------------------------ YamlParserOutput ---------------------------------
// ---------------------------------------------------------------------------------

yaml_differential_parser::YamlParserOutput::YamlParserOutput(std::string* info)
{
    this->data = info;
}

yaml_differential_parser::YamlParserOutput::~YamlParserOutput()
{
    if(this->data != nullptr)
    {
        delete this->data;
    }
}

bool yaml_differential_parser::YamlParserOutput::equivalent(ParserOutput* compared_object)
{
    return *(std::string*)this->getData() == *(std::string*)compared_object->getData();
}

void* yaml_differential_parser::YamlParserOutput::getData()
{
    return static_cast<void*> (this->data);
}

// ---------------------------------------------------------------------------------
// ---------------------------------- YamlParser -----------------------------------
// ---------------------------------------------------------------------------------

std::string yaml_differential_parser::YamlParser::getName()
{
    return "yaml";
}


void* yaml_differential_parser::YamlParser::parse(uint8_t*input, size_t input_size)
{
    return nullptr;
}

differential_parser::ParserOutput* yaml_differential_parser::YamlParser::normalize
    (void* input)
{   
    std::string* output = new std::string("yaml!");

    differential_parser::ParserOutput* returnMe = new
        yaml_differential_parser::YamlParserOutput (output);
    
    return returnMe;
}