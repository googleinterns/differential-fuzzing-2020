#ifndef TOY_FUZZ_STRING_OUTPUT_BASE_H
#define TOY_FUZZ_STRING_OUTPUT_BASE_H

#include "base_parser.h"

#include <string>

namespace toy_generic_string_helper
{
// ---------------------------------------------------------------------------------
// ------------------------- ToyFuzzGenericStringOutput ----------------------------
// ---------------------------------------------------------------------------------

class ToyFuzzGenericStringOutput 
    : virtual public differential_parser::NormalizedOutput
{
    private:
        std::string* data;
        std::unique_ptr<std::string> error;

    public:
        ToyFuzzGenericStringOutput(std::string* info, std::unique_ptr<std::string> error_code);
        ~ToyFuzzGenericStringOutput();
        bool equivalent(NormalizedOutput* compared_object);
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
        ~ToyFuzzGenericStringParser() {};
        std::string getName();
        void* parse(const uint8_t* input, size_t input_size, std::string* error_code);
        differential_parser::NormalizedOutput* normalize(void* input, std::unique_ptr<std::string> error_code);
};

// ---------------------------------------------------------------------------------
// ---------------------- Helper Compare Strings Method ----------------------------
// ---------------------------------------------------------------------------------

bool compareStrings(differential_parser::NormalizedOutput* compared_object_one, 
    differential_parser::NormalizedOutput* compared_object_two);
} // namespace toy_generic_string_helper


#endif