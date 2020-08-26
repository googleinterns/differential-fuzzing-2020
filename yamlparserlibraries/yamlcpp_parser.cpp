#include "yamlcpp_parser.h"

#include "./utils/comparison_utils.h"

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
        if (input != nullptr)
        {
            *yaml_cpp_loop_temp = YAML::LoadAll(std::string((const char*)input, input_size));
        }
        else
        {
            *error_code = "ERROR";
            return nullptr;
        }
    }
    catch (const std::exception& err)
    {
        *error_code = "ERROR";
    }

    return static_cast<void*>(yaml_cpp_loop_temp);
}

yaml_normalization::YamlNormalizedOutput* YamlCppParser::normalize
    (void* input, std::string* error_code)
{
    yaml_normalization::YamlNormalizedOutput* return_me = nullptr;

    if (std::vector<YAML::Node>* casted_input = static_cast<std::vector<YAML::Node>*>(input))
    {
        return_me = new
            yaml_normalization::YamlNormalizedOutput(casted_input, error_code);        
    }

    return return_me;
}
}