#include "libyaml_parser.h"

#include <iostream>

namespace libyaml_differential_parser
{
// ---------------------------------------------------------------------------------
// ---------------------------------- LibyamlParser -----------------------------------
// ---------------------------------------------------------------------------------

std::string LibyamlParser::getName()
{
    return "libyaml";
}


void* LibyamlParser::parse(const uint8_t* input, size_t input_size, std::string* error_code)
{
    return static_cast<void*>
            (libyaml_parsing::ParseLibyaml(input, input_size, error_code));
}

yaml_normalization::YamlNormalizedOutput* LibyamlParser::normalize
    (void* input, std::unique_ptr<std::string> error_code)
{
    if (std::vector<YAML::Node>* casted_input = static_cast<std::vector<YAML::Node>*>(input))
    {
        return new yaml_normalization::YamlNormalizedOutput(casted_input, std::move(error_code));
    }
    return nullptr;
}


// ---------------------------------------------------------------------------------
// ---------------------------------- Static Parts ---------------------------------
// ---------------------------------------------------------------------------------

libyaml_differential_parser::LibyamlParser
    libyaml_differential_parser::LibyamlParser::instance;

libyaml_differential_parser::LibyamlParser* LibyamlParser::GetStaticInstance()
{
    return &instance;
}
} // namespace yamlcpp_differential_parser