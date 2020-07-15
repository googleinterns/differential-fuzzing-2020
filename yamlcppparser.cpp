#include "yamlcppparser.h"

#include "yaml-cpp/yaml.h"
#include <iostream>

namespace yamlcpp_differential_parser
{
// ---------------------------------------------------------------------------------
// ------------------------------ YamlCppParserOutput ---------------------------------
// ---------------------------------------------------------------------------------

YamlCppParserOutput::YamlCppParserOutput(std::string* info, std::string* error_code)
{
    this->data = info;
    this->error = error_code;
}

YamlCppParserOutput::~YamlCppParserOutput()
{
    if (this->data != nullptr)
    {
        delete this->data;
    }
    delete this->error;
}

bool YamlCppParserOutput::equivalent(ParserOutput* compared_object)
{
    if(!this->getError()->empty() && !compared_object->getError()->empty())
    {
        if(*this->getError() == *compared_object->getError())
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    return *(std::string*)this->getData() == *(std::string*)compared_object->getData();
}

void* YamlCppParserOutput::getData()
{
    return static_cast<void*>(this->data);
}

std::string* YamlCppParserOutput::getError()
{
    return this->error;
}

// ---------------------------------------------------------------------------------
// ---------------------------------- YamlCppParser -----------------------------------
// ---------------------------------------------------------------------------------

std::string YamlCppParser::getName()
{
    return "yaml-cpp";
}


void* YamlCppParser::parse(const uint8_t* input, size_t input_size, std::string* error_code)
{
    std::string* yaml_cpp_loop_temp = new std::string;

    if(input_size>0)
    {
        try
        {
            YAML::Emitter out;

            out << std::string((const char*)input, input_size);

            *yaml_cpp_loop_temp = out.c_str();          
        }
        catch(const std::exception& e)
        {
            *error_code = e.what();
        }
        

    }
    return (void*)yaml_cpp_loop_temp;
}

differential_parser::ParserOutput* YamlCppParser::normalize
    (void* input, std::string* error_code)
{   
    differential_parser::ParserOutput* returnMe = new
        YamlCppParserOutput((std::string*)input, error_code);
    
    return returnMe;
}
}