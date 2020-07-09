#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <string>

#include "baseparser.h"

namespace xml_differential_parser
{
// ---------------------------------------------------------------------------------
// ------------------------------ XmlParserOutput ---------------------------------
// ---------------------------------------------------------------------------------

class XmlParserOutput 
    : virtual public differential_parser::ParserOutput
{

    private:
        std::string* data;

    public:
        XmlParserOutput(std::string*);
        ~XmlParserOutput();
        bool equivalent(ParserOutput*);
        void* getData();
};

// ---------------------------------------------------------------------------------
// ---------------------------------- YamlParser -----------------------------------
// ---------------------------------------------------------------------------------

class XmlParser : virtual public differential_parser::Parser
{
    public:
        std::string getName();
        void* parse(uint8_t*, size_t) ;
        differential_parser::ParserOutput* normalize(void*);
};
} // namespace xml_differential_parser


#endif