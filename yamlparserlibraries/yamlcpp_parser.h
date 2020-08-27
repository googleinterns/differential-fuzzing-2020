#ifndef YAMLCPP_PARSER_H
#define YAMLCPP_PARSER_H

#include "base_parser.h"
#include "yaml-cpp/yaml.h"
#include "yaml_normalized_output.h"

#include <string>

namespace yamlcpp_differential_parser
{

// ---------------------------------------------------------------------------------
// ---------------------------------- YamlCppParser -----------------------------------
// ---------------------------------------------------------------------------------
class YamlCppParser : virtual public differential_parser::Parser
{
    private:
        static YamlCppParser* instance;
    public:
        std::string getName();
        void* parse(const uint8_t* input, size_t input_size, std::unique_ptr<std::string>* error_code);
        yaml_normalization::YamlNormalizedOutput* normalize(void* input, std::unique_ptr<std::string>* error_code);
        static YamlCppParser* GetStaticInstance();

};
} // namespace yaml_differential_parser

#endif