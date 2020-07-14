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
        LibyamlParserOutput(std::string*, std::string*);
        ~LibyamlParserOutput();
        bool equivalent(ParserOutput*);
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
        void* parse(const uint8_t*, size_t, std::string*);
        differential_parser::ParserOutput* normalize(void*, std::string*);
};
} 


#endif