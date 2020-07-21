#include "toyfuzzstringoutputbase.h"

#include <iostream>

namespace toy_generic_string_helper
{
// ---------------------------------------------------------------------------------
// ------------------------- ToyFuzzGenericStringOutput ----------------------------
// ---------------------------------------------------------------------------------

ToyFuzzGenericStringOutput::ToyFuzzGenericStringOutput(std::string* info, std::string* error_code)
{
    this->data = info;
    this->error = error_code;
}

ToyFuzzGenericStringOutput::~ToyFuzzGenericStringOutput()
{
    if (this->data != nullptr)
    {
        delete this->data;
    }
    delete this->error;
}

bool ToyFuzzGenericStringOutput::equivalent(ParserOutput* compared_object)
{
    return toy_generic_string_helper::compareStrings(this, compared_object);
}

void* ToyFuzzGenericStringOutput::getData()
{
    return static_cast<void*>(this->data);
}

std::string* ToyFuzzGenericStringOutput::getError()
{
    return this->error;
}

// ---------------------------------------------------------------------------------
// ------------------------- ToyFuzzGenericStringParser ----------------------------
// ---------------------------------------------------------------------------------

ToyFuzzGenericStringParser::ToyFuzzGenericStringParser(std::string given_parser_modifier,
                std::string given_normalizer_modifier, std::string given_name_modifier,
                std::string given_error_modifier)
{
    this->parser_modifier = given_parser_modifier;
    this->normalizer_modifier = given_normalizer_modifier;
    this->name_modifier = given_name_modifier;
    this->error_modifier = given_error_modifier;
}

ToyFuzzGenericStringParser::~ToyFuzzGenericStringParser()
{

}

std::string ToyFuzzGenericStringParser::getName()
{
    return this->name_modifier;
}


void* ToyFuzzGenericStringParser::parse(const uint8_t* input, size_t input_size, std::string* error_code)
{
    std::string* toy_output = new std::string;

    *toy_output = std::string((const char*)input, input_size) + this->parser_modifier;

    *error_code = this->error_modifier;

    return (void*)toy_output;
}

differential_parser::ParserOutput* ToyFuzzGenericStringParser::normalize
    (void* input, std::string* error_code)
{   
    *(std::string*)input = *(std::string*)input + this->normalizer_modifier;
    differential_parser::ParserOutput* returnMe = new
        toy_generic_string_helper::ToyFuzzGenericStringOutput((std::string*)input, error_code);
    
    return returnMe;
}


// ---------------------------------------------------------------------------------
// ---------------------- Helper Compare Strings Method ----------------------------
// ---------------------------------------------------------------------------------

bool compareStrings(differential_parser::ParserOutput* compared_object_one, 
    differential_parser::ParserOutput* compared_object_two)
{
    if (!compared_object_two->getError()->empty() && !compared_object_one->getError()->empty())
    {
        if (*compared_object_two->getError() == *compared_object_one->getError())
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else if (!compared_object_two->getError()->empty() || !compared_object_one->getError()->empty())
    {
        return false;
    }
    return *(std::string*)compared_object_two->getData() == *(std::string*)compared_object_one->getData();
}
}