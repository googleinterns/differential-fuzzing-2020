#include "toyfuzz4parser.h"

#include <iostream>

namespace toy4_differential_parser
{
// ---------------------------------------------------------------------------------
// ------------------------------ Toy4ParserOutput ---------------------------------
// ---------------------------------------------------------------------------------

Toy4ParserOutput::Toy4ParserOutput(std::string* info, std::string* error_code)
{
    this->data = info;
    this->error = error_code;
}

Toy4ParserOutput::~Toy4ParserOutput()
{
    if (this->data != nullptr)
    {
        delete this->data;
    }
    delete this->error;
}

bool Toy4ParserOutput::equivalent(ParserOutput* compared_object)
{
    if (!this->getError()->empty() && !compared_object->getError()->empty())
    {
        if(*this->getError() == *compared_object->getError())
        {
            return true;
        }
        else
        {
            return false;
        }
    }else if (!this->getError()->empty() || !compared_object->getError()->empty())
    {
        return false;
    }
    return *(std::string*)this->getData() == *(std::string*)compared_object->getData();
}

void* Toy4ParserOutput::getData()
{
    return static_cast<void*>(this->data);
}

std::string* Toy4ParserOutput::getError()
{
    return this->error;
}

// ---------------------------------------------------------------------------------
// ---------------------------------- Toy4Parser -----------------------------------
// ---------------------------------------------------------------------------------

std::string Toy4Parser::getName()
{
    return "toy-4";
}


void* Toy4Parser::parse(const uint8_t* input, size_t input_size, std::string* error_code)
{
    std::string* toy_output = new std::string;

    *toy_output = std::string((const char*)input, input_size) + " wears a hot hat";

    *error_code = "ERROR: warm hat";

    return (void*)toy_output;
}

differential_parser::ParserOutput* Toy4Parser::normalize
    (void* input, std::string* error_code)
{   
    differential_parser::ParserOutput* returnMe = new
        Toy4ParserOutput((std::string*)input, error_code);
    
    return returnMe;
}
}