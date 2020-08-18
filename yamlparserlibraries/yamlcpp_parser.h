#ifndef YAMLCPP_PARSER_H
#define YAMLCPP_PARSER_H

#include "base_parser.h"

#include <string>

namespace yamlcpp_differential_parser
{
// ---------------------------------------------------------------------------------
// ------------------------------ YamlCppNormalizedOutput ---------------------------------
// ---------------------------------------------------------------------------------

class YamlCppNormalizedOutput 
    : virtual public differential_parser::NormalizedOutput
{
    private:
        std::string* data;
        std::string* error;
    public:
        YamlCppNormalizedOutput(std::string* info, std::string* error_code);
        ~YamlCppNormalizedOutput();
        bool equivalent(NormalizedOutput* compared_object);
        void* getData();
        std::string* getError();
};

// ---------------------------------------------------------------------------------
// ---------------------------------- YamlCppParser -----------------------------------
// ---------------------------------------------------------------------------------

class YamlCppParser : virtual public differential_parser::Parser
{
    
    public:
        std::string getName();
        void* parse(const uint8_t* input, size_t input_size, std::string* error_code);
        differential_parser::NormalizedOutput* normalize(void* input, std::string* error_code);
};
} // namespace yaml_differential_parser


#endif