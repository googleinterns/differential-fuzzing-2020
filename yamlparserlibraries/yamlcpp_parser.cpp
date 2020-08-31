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

void* YamlCppParser::parse(const uint8_t* input, size_t input_size, std::unique_ptr<std::string>* error_code)
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
            *error_code = std::unique_ptr<std::string>(new std::string("ERROR"));;
            return nullptr;
        }
    }
    catch (const std::exception& err)
    {
        *error_code = std::unique_ptr<std::string>(new std::string("ERROR"));;
    }

    return static_cast<void*>(yaml_cpp_loop_temp);
}

yaml_normalization::YamlNormalizedOutput* YamlCppParser::normalize
    (void* input, std::unique_ptr<std::string>* error_code)
{
    if (std::vector<YAML::Node>* casted_input = static_cast<std::vector<YAML::Node>*>(input))
    {
        return new
            yaml_normalization::YamlNormalizedOutput(casted_input, error_code);
    }

    return nullptr;
}

// ---------------------------------------------------------------------------------
// ---------------------------------- Static Parts ---------------------------------
// ---------------------------------------------------------------------------------

yamlcpp_differential_parser::YamlCppParser 
    yamlcpp_differential_parser::YamlCppParser::instance;

yamlcpp_differential_parser::YamlCppParser* YamlCppParser::GetStaticInstance()
{
    return &instance;
}
} // namespace yamlcpp_differential_parser