#ifndef PARSER_H
#define PARSER_H

#include <string>

namespace differential_parser
{
    class Parser;
}

class differential_parser::Parser
{
    public:
        virtual std::string getName() = 0;
        virtual void* parse(uint8_t*, size_t) = 0;
        virtual void* normalize(void*) = 0;
        virtual bool equivalent(void*,void*) = 0;
};

#endif