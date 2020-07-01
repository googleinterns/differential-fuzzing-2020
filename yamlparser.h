#ifndef YAMLPARSER_H
#define YAMLPARSER_H

#include <string>

#include "parser.cpp"

class YamlParser : virtual public Parser
{
    private:
    public:
        void* parse(uint8_t*, size_t);
        void*normalize(void*);
        bool equivalent(void*, void*);
};

#endif