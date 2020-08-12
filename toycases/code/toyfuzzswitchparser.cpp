#include "toyfuzzswitchparser.h"
#include "toyfuzzstringoutputbase.h"

#include <iostream>

namespace toy_switch_differential_parser
{
// ---------------------------------------------------------------------------------
// ---------------------------------- ToySwitchParser -----------------------------------
// ---------------------------------------------------------------------------------

std::string ToySwitchParser::getName()
{
    return "toy-Switch";
}


void* ToySwitchParser::parse(const uint8_t* input, size_t input_size, std::string* error_code)
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

differential_parser::ParserOutput* ToySwitchParser::normalize
    (void* input, std::string* error_code)
{   
    *(std::string*)input = *(std::string*)input + "pair of glasses";
    differential_parser::ParserOutput* returnMe = new
        toy_generic_string_helper::ToyFuzzGenericStringOutput((std::string*)input, error_code);

    return returnMe;
}
}