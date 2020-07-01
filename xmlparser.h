#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <string>

#include "parser.h"

namespace xaml_differential_parser
{
    class XmlParser;
}


class xaml_differential_parser::XmlParser : virtual public differential_parser::Parser
{
    public:
        std::string getName();
        void* parse(uint8_t*, size_t) ;
        void* normalize(void*);
        bool equivalent(void*, void*);
};

#endif