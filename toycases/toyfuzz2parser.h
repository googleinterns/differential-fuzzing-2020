#ifndef TOYFUZZ2PARSER_H
#define TOYFUZZ2PARSER_H

#include "baseparser.h"

#include <string>

namespace toy2_differential_parser
{
// ---------------------------------------------------------------------------------
// ------------------------------ Toy2ParserOutput ---------------------------------
// ---------------------------------------------------------------------------------

class Toy2ParserOutput 
    : virtual public differential_parser::ParserOutput
{
    private:
        std::string* data;
        std::string* error;
    public:
        Toy2ParserOutput(std::string* info, std::string* error_code);
        ~Toy2ParserOutput();
        bool equivalent(ParserOutput* compared_object);
        void* getData();
        std::string* getError();
};

// ---------------------------------------------------------------------------------
// ---------------------------------- Toy2Parser -----------------------------------
// ---------------------------------------------------------------------------------

class Toy2Parser : virtual public differential_parser::Parser
{
    
    public:
        std::string getName();
        void* parse(const uint8_t* input, size_t input_size, std::string* error_code);
        differential_parser::ParserOutput* normalize(void* input, std::string* error_code);
};
} // namespace yaml_differential_parser


#endif