#ifndef TOYFUZZ6PARSER_H
#define TOYFUZZ6PARSER_H

#include "baseparser.h"

#include <string>

namespace toy6_differential_parser
{
// ---------------------------------------------------------------------------------
// ------------------------------ Toy6ParserOutput ---------------------------------
// ---------------------------------------------------------------------------------

class Toy6ParserOutput 
    : virtual public differential_parser::ParserOutput
{
    private:
        std::string* data;
        std::string* error;
    public:
        Toy6ParserOutput(std::string* info, std::string* error_code);
        ~Toy6ParserOutput();
        bool equivalent(ParserOutput* compared_object);
        void* getData();
        std::string* getError();
};

// ---------------------------------------------------------------------------------
// ---------------------------------- Toy6Parser -----------------------------------
// ---------------------------------------------------------------------------------

class Toy6Parser : virtual public differential_parser::Parser
{
    public:
        std::string getName();
        void* parse(const uint8_t* input, size_t input_size, std::string* error_code);
        differential_parser::ParserOutput* normalize(void* input, std::string* error_code);
};
} // namespace yaml_differential_parser


#endif