#include "toyfuzz6parser.h"

#include <iostream>

namespace toy6_differential_parser
{
// ---------------------------------------------------------------------------------
// ------------------------------ Toy6ParserOutput ---------------------------------
// ---------------------------------------------------------------------------------

Toy6ParserOutput::Toy6ParserOutput(std::string* info, std::string* error_code)
{
    this->data = info;
    this->error = error_code;
}

Toy6ParserOutput::~Toy6ParserOutput()
{
    if (this->data != nullptr)
    {
        delete this->data;
    }
    delete this->error;
}

bool Toy6ParserOutput::equivalent(ParserOutput* compared_object)
{
    if(!this->getError()->empty() && !compared_object->getError()->empty())
    {
        if(*this->getError() == *compared_object->getError())
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    return *(std::string*)this->getData() == *(std::string*)compared_object->getData();
}

void* Toy6ParserOutput::getData()
{
    return static_cast<void*>(this->data);
}

std::string* Toy6ParserOutput::getError()
{
    return this->error;
}

// ---------------------------------------------------------------------------------
// ---------------------------------- Toy6Parser -----------------------------------
// ---------------------------------------------------------------------------------

std::string Toy6Parser::getName()
{
    return "toy-6";
}


void* Toy6Parser::parse(const uint8_t* input, size_t input_size, std::string* error_code)
{
    std::string* toy_output = new std::string;

    *toy_output = std::string((const char*)input, input_size) + " wears a floatty hat";

    *error_code = "ERROR: hat too hot";

    return (void*)toy_output;
}

differential_parser::ParserOutput* Toy6Parser::normalize
    (void* input, std::string* error_code)
{   
    differential_parser::ParserOutput* returnMe = new
        Toy6ParserOutput((std::string*)input, error_code);
    
    return returnMe;
}
}