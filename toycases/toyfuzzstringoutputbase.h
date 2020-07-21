#ifndef TOYFUZZSTRINGOUTPUTBASE_H
#define TOYFUZZSTRINGOUTPUTBASE_H

#include "baseparser.h"

#include <string>

namespace toy_generic_string_helper
{
// ---------------------------------------------------------------------------------
// ------------------------- ToyFuzzGenericStringOutput ----------------------------
// ---------------------------------------------------------------------------------

class ToyFuzzGenericStringOutput 
    : virtual public differential_parser::ParserOutput
{
    private:
        std::string* data;
        std::string* error;
    public:
        ToyFuzzGenericStringOutput(std::string* info, std::string* error_code);
        ~ToyFuzzGenericStringOutput();
        bool equivalent(ParserOutput* compared_object);
        void* getData();
        std::string* getError();
};

// ---------------------------------------------------------------------------------
// ---------------------------------- Toy8Parser -----------------------------------
// ---------------------------------------------------------------------------------

class ToyFuzzGenericStringParser : virtual public differential_parser::Parser
{
    private:
        std::string parser_modifier = "";
        std::string normalizer_modifier = "";
        std::string name_modifier = "";
        std::string error_modifier = "";

    public:
        ToyFuzzGenericStringParser(std::string given_parser_modifier,
                std::string given_normalizer_modifier, std::string given_name_modifier,
                std::string given_error_modifier);
        ~ToyFuzzGenericStringParser();
        std::string getName();
        void* parse(const uint8_t* input, size_t input_size, std::string* error_code);
        differential_parser::ParserOutput* normalize(void* input, std::string* error_code);
};

// ---------------------------------------------------------------------------------
// ---------------------- Helper Compare Strings Method ----------------------------
// ---------------------------------------------------------------------------------

bool compareStrings(differential_parser::ParserOutput* compared_object_one, 
    differential_parser::ParserOutput* compared_object_two);
} // namespace yaml_differential_parser


#endif