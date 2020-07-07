#ifndef PARSER_H
#define PARSER_H

#include <string>

namespace differential_parser
{
    class Parser;
    class ParserOutput;
}

class differential_parser::ParserOutput
{
    public:
        virtual ~ParserOutput(){};
        virtual bool equivalent(ParserOutput*) = 0;
        virtual void* getData() = 0;
};

class differential_parser::Parser
{
    public:
        virtual std::string getName() = 0;
        virtual void* parse(uint8_t*, size_t) = 0;
        virtual differential_parser::ParserOutput* normalize(void*) = 0;
};


#endif