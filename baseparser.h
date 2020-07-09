#ifndef PARSER_H
#define PARSER_H

#include <string>

namespace differential_parser
{
class ParserOutput
{
    public:
        virtual ~ParserOutput(){};
        virtual bool equivalent(ParserOutput*) = 0;
        virtual void* getData() = 0;
};

class Parser
{
    public:
        virtual std::string getName() = 0;
        virtual void* parse(uint8_t*, size_t) = 0;
        virtual differential_parser::ParserOutput* normalize(void*) = 0;
};
} // namespace differential_parser


#endif