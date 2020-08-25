#include "yamlcpp_parser.h"

#include "yaml-cpp/yaml.h"

#include "./utils/comparison_utils.h"
#include <iostream>
#include <string>

namespace yamlcpp_differential_parser
{
// ---------------------------------------------------------------------------------
// ------------------------------ YamlCppNormalizedOutput --------------------------
// ---------------------------------------------------------------------------------

YamlCppNormalizedOutput::YamlCppNormalizedOutput(std::vector<YAML::Node>* info, std::string* error_code)
{
    this->data = info;
    this->error = error_code;
}

YamlCppNormalizedOutput::~YamlCppNormalizedOutput()
{
    if (this->data != nullptr)
    {
        delete this->data;
    }
    delete this->error;
}

bool YamlCppNormalizedOutput::equivalent(NormalizedOutput* compared_object)
{
    if (!(*this->getError()).empty() || !(*compared_object->getError()).empty())
    {
        return (*this->getError() == *compared_object->getError());
    }
    else
    {
        return CompareMultipleNodes
            ((std::vector<YAML::Node>*)this->getData(), (std::vector<YAML::Node>*)compared_object->getData());
    }
}

void* YamlCppNormalizedOutput::getData()
{
    return static_cast<void*>(this->data);
}

std::string* YamlCppNormalizedOutput::getError()
{
    return this->error;
}

// ---------------------------------------------------------------------------------
// ---------------------------------- YamlCppParser --------------------------------
// ---------------------------------------------------------------------------------

std::string YamlCppParser::getName()
{
    return "yaml-cpp";
}

void* YamlCppParser::parse(const uint8_t* input, size_t input_size, std::string* error_code)
{
    std::vector<YAML::Node>* yaml_cpp_loop_temp = new std::vector<YAML::Node>;

    try
    {
        *yaml_cpp_loop_temp = YAML::LoadAll(std::string((const char*)input, input_size));
    }
    catch (const std::exception& err)
    {
        *error_code = "ERROR";
    }

    return (void*) yaml_cpp_loop_temp;
}

differential_parser::NormalizedOutput* YamlCppParser::normalize
    (void* input, std::string* error_code)
{   
    differential_parser::NormalizedOutput* returnMe = new
        YamlCppNormalizedOutput((std::vector<YAML::Node>*)input, error_code);
    
    return returnMe;
}
}