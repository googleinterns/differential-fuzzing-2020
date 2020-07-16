#ifndef TOYFUZZ1PARSER_H
#define TOYFUZZ1PARSER_H

#include "baseparser.h"

#include <string>

namespace toy1_differential_parser
{
// ---------------------------------------------------------------------------------
// ------------------------------ Toy1ParserOutput ---------------------------------
// ---------------------------------------------------------------------------------

class Toy1ParserOutput 
    : virtual public differential_parser::ParserOutput
{
    private:
        std::string* data;
        std::string* error;
    public:
        Toy1ParserOutput(std::string* info, std::string* error_code);
        ~Toy1ParserOutput();
        bool equivalent(ParserOutput* compared_object);
        void* getData();
        std::string* getError();
};

// ---------------------------------------------------------------------------------
// ---------------------------------- Toy1Parser -----------------------------------
// ---------------------------------------------------------------------------------

class Toy1Parser : virtual public differential_parser::Parser
{
    
    public:
        std::string getName();
        void* parse(const uint8_t* input, size_t input_size, std::string* error_code);
        differential_parser::ParserOutput* normalize(void* input, std::string* error_code);
};
} // namespace yaml_differential_parser


#endif