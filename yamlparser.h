#ifndef YAMLPARSER_H
#define YAMLPARSER_H

#include <string>

#include "parser.h"

class YamlParser : virtual public Parser
{
    public:
        std::string getName();
        void* parse(uint8_t*, size_t);
        void*normalize(void*);
        bool equivalent(void*, void*);
};

#endif