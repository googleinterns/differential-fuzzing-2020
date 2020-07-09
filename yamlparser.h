#ifndef YAMLPARSER_H
#define YAMLPARSER_H

#include "baseparser.h"

#include <string>

namespace yaml_differential_parser
{
// ---------------------------------------------------------------------------------
// ------------------------------ YamlParserOutput ---------------------------------
// ---------------------------------------------------------------------------------

class YamlParserOutput 
    : virtual public differential_parser::ParserOutput
{
    private:
        std::string* data;
    public:
        YamlParserOutput(std::string*);
        ~YamlParserOutput();
        bool equivalent(ParserOutput*);
        void* getData();
};

// ---------------------------------------------------------------------------------
// ---------------------------------- YamlParser -----------------------------------
// ---------------------------------------------------------------------------------

class YamlParser : virtual public differential_parser::Parser
{
    
    public:
        std::string getName();
        void* parse(uint8_t*, size_t);
        differential_parser::ParserOutput* normalize(void*);
};
} // namespace yaml_differential_parser


#endif