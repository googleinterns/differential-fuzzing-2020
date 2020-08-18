#ifndef LIBYAMLPARSER_H
#define LIBYAMLPARSER_H

#include "base_parser.h"

#include <string>

namespace libyaml_differential_parser
{
// ---------------------------------------------------------------------------------
// ------------------------------ LibyamlNormalizedOutput ---------------------------------
// ---------------------------------------------------------------------------------

class LibyamlNormalizedOutput 
    : virtual public differential_parser::NormalizedOutput
{
    private:
        std::string* data;
        std::string* error;
    public:
        LibyamlNormalizedOutput(std::string* info, std::string* error_code);
        ~LibyamlNormalizedOutput();
        bool equivalent(NormalizedOutput* compared_object);
        void* getData();
        std::string* getError();
};

// ---------------------------------------------------------------------------------
// ---------------------------------- LibyamlParser -----------------------------------
// ---------------------------------------------------------------------------------

class LibyamlParser : virtual public differential_parser::Parser
{
    public:
        std::string getName();
        void* parse(const uint8_t* input, size_t input_size, std::string* error_code);
        differential_parser::NormalizedOutput* normalize(void* input, std::string* error_code);
};
} 


#endif