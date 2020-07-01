#ifndef YAMLPARSER_H
#define YAMLPARSER_H

#include "parser.h"

#include <string>
namespace yaml_differential_parser
{
    class YamlParser;
}

class yaml_differential_parser::YamlParser : virtual public differential_parser::Parser
{
    public:
        std::string getName();
        void* parse(uint8_t*, size_t);
        void*normalize(void*);
        bool equivalent(void*, void*);
};

#endif