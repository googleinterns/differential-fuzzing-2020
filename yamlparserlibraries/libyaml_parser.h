#ifndef LIBYAML_PARSER_H
#define LIBYAML_PARSER_H

#include "base_parser.h"
#include "yaml_normalized_output.h"
#include "utils/libyaml_utils.h"

#include "include/yaml.h"
#include <string>

namespace libyaml_differential_parser
{
// ---------------------------------------------------------------------------------
// ---------------------------------- LibyamlParser -----------------------------------
// ---------------------------------------------------------------------------------

class LibyamlParser : virtual public differential_parser::Parser
{
    private:
        static LibyamlParser instance;
    public:
        std::string getName();
        void* parse(const uint8_t* input, size_t input_size, std::unique_ptr<std::string>* error_code);
        yaml_normalization::YamlNormalizedOutput* normalize(void* input, std::unique_ptr<std::string>* error_code);
        static LibyamlParser* GetStaticInstance();
};
} 


#endif