#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <string>

#include "parser.cpp"

class XmlParser : virtual public Parser
{
    private:
    public:
        void* parse(uint8_t*, size_t) ;
        void* normalize(void*);
        bool equivalent(void*, void*);
};

#endif