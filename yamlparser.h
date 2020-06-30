#ifndef YAMLPARSER_H
#define YAMLPARSER_H

#include <string>

#include "parser.h"

class yamlparser : virtual public parser
{
    private:
    public:
        uint8_t* parse(uint8_t*, size_t);
        std::string normalize(uint8_t*);
};

#endif