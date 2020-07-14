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
        virtual std::string* getError() = 0;
};

class Parser
{
    public:
        virtual std::string getName() = 0;
        virtual void* parse(const uint8_t*, size_t, std::string*) = 0;
        virtual differential_parser::ParserOutput* normalize(void*,std::string*) = 0;
};
} // namespace differential_parser


#endif