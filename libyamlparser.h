#ifndef LIBYAMLPARSER_H
#define LIBYAMLPARSER_H

#include "baseparser.h"

#include <string>

namespace libyaml_differential_parser
{
// ---------------------------------------------------------------------------------
// ------------------------------ LibyamlParserOutput ---------------------------------
// ---------------------------------------------------------------------------------

class LibyamlParserOutput 
    : virtual public differential_parser::ParserOutput
{
    private:
        std::string* data;
        std::string* error;
    public:
        LibyamlParserOutput(std::string* info, std::string* error_code);
        ~LibyamlParserOutput();
        bool equivalent(ParserOutput* compared_object);
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
        differential_parser::ParserOutput* normalize(void* input, std::string* error_code);
};
} 


#endif