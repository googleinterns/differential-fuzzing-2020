#ifndef TOYFUZZ7PARSER_H
#define TOYFUZZ7PARSER_H

#include "baseparser.h"

#include <string>

namespace toy7_differential_parser
{
// ---------------------------------------------------------------------------------
// ------------------------------ Toy7ParserOutput ---------------------------------
// ---------------------------------------------------------------------------------

class Toy7ParserOutput 
    : virtual public differential_parser::ParserOutput
{
    private:
        int* data;
        std::string* error;
    public:
        Toy7ParserOutput(int* info, std::string* error_code);
        ~Toy7ParserOutput();
        bool equivalent(ParserOutput* compared_object);
        void* getData();
        std::string* getError();
};

// ---------------------------------------------------------------------------------
// ---------------------------------- Toy7Parser -----------------------------------
// ---------------------------------------------------------------------------------

class Toy7Parser : virtual public differential_parser::Parser
{
    public:
        std::string getName();
        void* parse(const uint8_t* input, size_t input_size, std::string* error_code);
        differential_parser::ParserOutput* normalize(void* input, std::string* error_code);
};
} // namespace yaml_differential_parser


#endif