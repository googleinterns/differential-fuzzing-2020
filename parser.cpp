#ifndef PARSER_H
#define PARSER_H

#include <string>

class Parser
{
    public:
        std::string name = "NA";

        virtual void* parse(uint8_t*, size_t) = 0;
        virtual void* normalize(void*) = 0;
        virtual bool equivalent(void*,void*) = 0;
};

#endif