#ifndef TOYFUZZ8PARSER_H
#define TOYFUZZ8PARSER_H

#include "baseparser.h"

#include <string>

namespace toy8_differential_parser
{
// ---------------------------------------------------------------------------------
// ------------------------------ Toy8ParserOutput ---------------------------------
// ---------------------------------------------------------------------------------

class Toy8ParserOutput 
    : virtual public differential_parser::ParserOutput
{
    private:
        std::string* data;
        std::string* error;
    public:
        Toy8ParserOutput(std::string* info, std::string* error_code);
        ~Toy8ParserOutput();
        bool equivalent(ParserOutput* compared_object);
        void* getData();
        std::string* getError();
};

// ---------------------------------------------------------------------------------
// ---------------------------------- Toy8Parser -----------------------------------
// ---------------------------------------------------------------------------------

class Toy8Parser : virtual public differential_parser::Parser
{
    
    public:
        std::string getName();
        void* parse(const uint8_t* input, size_t input_size, std::string* error_code);
        differential_parser::ParserOutput* normalize(void* input, std::string* error_code);
};
} // namespace yaml_differential_parser


#endif