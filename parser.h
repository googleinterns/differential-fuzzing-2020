#ifndef PARSER_H
#define PARSER_H

#include <string>

class parser
{
    public:
        std::string name = "NA";

        virtual uint8_t* parse(uint8_t*, size_t) = 0;
        virtual std::string normalize(uint8_t*) = 0 ;
};

#endif