#include "yamlcpp_parser.h"

#include "yaml-cpp/yaml.h"

#include "./utils/comparison_utils.h"
#include <iostream>
#include <string>

namespace yamlcpp_differential_parser
{
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

yaml_normalization::YamlNormalizedOutput* YamlCppParser::normalize
    (void* input, std::string* error_code)
{   
    yaml_normalization::YamlNormalizedOutput* returnMe = new
        yaml_normalization::YamlNormalizedOutput((std::vector<YAML::Node>*)input, error_code);
    
    return returnMe;
}
}