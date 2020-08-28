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


void* LibyamlParser::parse(const uint8_t* input, size_t input_size, std::unique_ptr<std::string>* error_code)
{
    return ;
}

yaml_normalization::YamlNormalizedOutput* LibyamlParser::normalize
    (void* input, std::unique_ptr<std::string>* error_code)
{   
    return return_me;
}
}