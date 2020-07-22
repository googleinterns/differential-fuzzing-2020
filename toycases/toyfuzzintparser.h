#ifndef TOYFUZZINTPARSER_H
#define TOYFUZZINTPARSER_H

#include "baseparser.h"

#include <string>

namespace toy_int_differential_parser
{
// ---------------------------------------------------------------------------------
// ------------------------------ ToyIntParserOutput ---------------------------------
// ---------------------------------------------------------------------------------

class ToyIntParserOutput 
    : virtual public differential_parser::ParserOutput
{
    private:
        int* data;
        std::string* error;
    public:
        ToyIntParserOutput(int* info, std::string* error_code);
        ~ToyIntParserOutput();
        bool equivalent(ParserOutput* compared_object);
        void* getData();
        std::string* getError();
};

// ---------------------------------------------------------------------------------
// ---------------------------------- ToyIntParser -----------------------------------
// ---------------------------------------------------------------------------------

class ToyIntParser : virtual public differential_parser::Parser
{
    public:
        std::string getName();
        void* parse(const uint8_t* input, size_t input_size, std::string* error_code);
        differential_parser::ParserOutput* normalize(void* input, std::string* error_code);
};
} // namespace yaml_differential_parser


#endif