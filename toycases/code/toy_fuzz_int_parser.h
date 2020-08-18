#ifndef TOY_FUZZ_INT_PARSER_H
#define TOY_FUZZ_INT_PARSER_H

#include "base_parser.h"

#include <string>

namespace toy_int_differential_parser
{
// ---------------------------------------------------------------------------------
// ------------------------------ ToyIntParserOutput ---------------------------------
// ---------------------------------------------------------------------------------

class ToyIntParserOutput 
    : virtual public differential_parser::NormalizedOutput
{
    private:
        int* data;
        std::string* error;
    public:
        ToyIntParserOutput(int* info, std::string* error_code);
        ~ToyIntParserOutput();
        bool equivalent(NormalizedOutput* compared_object);
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
        differential_parser::NormalizedOutput* normalize(void* input, std::string* error_code);
};
} // namespace toy_int_differential_parser


#endif