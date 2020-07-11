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
    public:
        LibyamlParserOutput(std::string*);
        ~LibyamlParserOutput();
        bool equivalent(ParserOutput*);
        void* getData();
};

// ---------------------------------------------------------------------------------
// ---------------------------------- LibyamlParser -----------------------------------
// ---------------------------------------------------------------------------------

class LibyamlParser : virtual public differential_parser::Parser
{
    
    public:
        std::string getName();
        void* parse(uint8_t*, size_t);
        differential_parser::ParserOutput* normalize(void*);
};
} // namespace yaml_differential_parser


#endif