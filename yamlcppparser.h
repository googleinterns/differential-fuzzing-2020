#ifndef YAMLCPPPARSER_H
#define YAMLCPPPARSER_H

#include "baseparser.h"

#include <string>

namespace yamlcpp_differential_parser
{
// ---------------------------------------------------------------------------------
// ------------------------------ YamlCppParserOutput ---------------------------------
// ---------------------------------------------------------------------------------

class YamlCppParserOutput 
    : virtual public differential_parser::ParserOutput
{
    private:
        std::string* data;
    public:
        YamlCppParserOutput(std::string*);
        ~YamlCppParserOutput();
        bool equivalent(ParserOutput*);
        void* getData();
};

// ---------------------------------------------------------------------------------
// ---------------------------------- YamlCppParser -----------------------------------
// ---------------------------------------------------------------------------------

class YamlCppParser : virtual public differential_parser::Parser
{
    
    public:
        std::string getName();
        void* parse(const uint8_t*, size_t);
        differential_parser::ParserOutput* normalize(void*);
};
} // namespace yaml_differential_parser


#endif