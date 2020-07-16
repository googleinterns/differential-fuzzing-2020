#ifndef TOYFUZZ4PARSER_H
#define TOYFUZZ4PARSER_H

#include "baseparser.h"

#include <string>

namespace toy4_differential_parser
{
// ---------------------------------------------------------------------------------
// ------------------------------ Toy4ParserOutput ---------------------------------
// ---------------------------------------------------------------------------------

class Toy4ParserOutput 
    : virtual public differential_parser::ParserOutput
{
    private:
        std::string* data;
        std::string* error;
    public:
        Toy4ParserOutput(std::string* info, std::string* error_code);
        ~Toy4ParserOutput();
        bool equivalent(ParserOutput* compared_object);
        void* getData();
        std::string* getError();
};

// ---------------------------------------------------------------------------------
// ---------------------------------- Toy4Parser -----------------------------------
// ---------------------------------------------------------------------------------

class Toy4Parser : virtual public differential_parser::Parser
{
    
    public:
        std::string getName();
        void* parse(const uint8_t* input, size_t input_size, std::string* error_code);
        differential_parser::ParserOutput* normalize(void* input, std::string* error_code);
};
} // namespace yaml_differential_parser


#endif