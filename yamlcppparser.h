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
        std::string* error;
    public:
        YamlCppParserOutput(std::string*, std::string*);
        ~YamlCppParserOutput();
        bool equivalent(ParserOutput*);
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
        void* parse(const uint8_t*, size_t, std::string*);
        differential_parser::ParserOutput* normalize(void*, std::string*);
};
} // namespace yaml_differential_parser


#endif