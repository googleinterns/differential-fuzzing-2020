#ifndef TOYFUZZ3PARSER_H
#define TOYFUZZ3PARSER_H

#include "baseparser.h"

#include <string>

namespace toy3_differential_parser
{
// ---------------------------------------------------------------------------------
// ------------------------------ Toy3ParserOutput ---------------------------------
// ---------------------------------------------------------------------------------

class Toy3ParserOutput 
    : virtual public differential_parser::ParserOutput
{
    private:
        std::string* data;
        std::string* error;
    public:
        Toy3ParserOutput(std::string* info, std::string* error_code);
        ~Toy3ParserOutput();
        bool equivalent(ParserOutput* compared_object);
        void* getData();
        std::string* getError();
};

// ---------------------------------------------------------------------------------
// ---------------------------------- Toy3Parser -----------------------------------
// ---------------------------------------------------------------------------------

class Toy3Parser : virtual public differential_parser::Parser
{
    
    public:
        std::string getName();
        void* parse(const uint8_t* input, size_t input_size, std::string* error_code);
        differential_parser::ParserOutput* normalize(void* input, std::string* error_code);
};
} // namespace yaml_differential_parser


#endif