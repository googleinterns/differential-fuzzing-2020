#include "toyfuzz9parser.h"
#include "toyfuzzstringoutputbase.h"

#include <iostream>

namespace toy9_differential_parser
{
// ---------------------------------------------------------------------------------
// ---------------------------------- Toy9Parser -----------------------------------
// ---------------------------------------------------------------------------------

std::string Toy9Parser::getName()
{
    return "toy-9";
}


void* Toy9Parser::parse(const uint8_t* input, size_t input_size, std::string* error_code)
{
    char deciding_factor = input[0];
    std::string* toy_output = new std::string;

    if(*input == '0')
    {
        *toy_output = std::string((const char*)input, input_size) + " wears a hot ";
    }
    else
    {
        *toy_output = std::string((const char*)input, input_size) + " wears a pink ";
    }

    return (void*)toy_output;
}

differential_parser::ParserOutput* Toy9Parser::normalize
    (void* input, std::string* error_code)
{   
    *(std::string*)input = *(std::string*)input + "pair of glasses";
    differential_parser::ParserOutput* returnMe = new
        toy_generic_string_helper::ToyFuzzGenericStringOutput((std::string*)input, error_code);
    
    std::cerr << *(std::string*)input << std::endl;

    return returnMe;
}
}