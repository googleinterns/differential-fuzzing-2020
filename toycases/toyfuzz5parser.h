#ifndef TOYFUZZ5PARSER_H
#define TOYFUZZ5PARSER_H

#include "baseparser.h"

#include <string>

namespace toy5_differential_parser
{
// ---------------------------------------------------------------------------------
// ------------------------------ Toy5ParserOutput ---------------------------------
// ---------------------------------------------------------------------------------

class Toy5ParserOutput 
    : virtual public differential_parser::ParserOutput
{
    private:
        std::string* data;
        std::string* error;
    public:
        Toy5ParserOutput(std::string* info, std::string* error_code);
        ~Toy5ParserOutput();
        bool equivalent(ParserOutput* compared_object);
        void* getData();
        std::string* getError();
};

// ---------------------------------------------------------------------------------
// ---------------------------------- Toy5Parser -----------------------------------
// ---------------------------------------------------------------------------------

class Toy5Parser : virtual public differential_parser::Parser
{
    public:
        std::string getName();
        void* parse(const uint8_t* input, size_t input_size, std::string* error_code);
        differential_parser::ParserOutput* normalize(void* input, std::string* error_code);
};
} // namespace yaml_differential_parser


#endif