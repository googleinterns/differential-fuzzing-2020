#include "toyfuzz3parser.h"

#include <iostream>

namespace toy3_differential_parser
{
// ---------------------------------------------------------------------------------
// ------------------------------ Toy3ParserOutput ---------------------------------
// ---------------------------------------------------------------------------------

Toy3ParserOutput::Toy3ParserOutput(std::string* info, std::string* error_code)
{
    this->data = info;
    this->error = error_code;
}

Toy3ParserOutput::~Toy3ParserOutput()
{
    if (this->data != nullptr)
    {
        delete this->data;
    }
    delete this->error;
}

bool Toy3ParserOutput::equivalent(ParserOutput* compared_object)
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

void* Toy3ParserOutput::getData()
{
    return static_cast<void*>(this->data);
}

std::string* Toy3ParserOutput::getError()
{
    return this->error;
}

// ---------------------------------------------------------------------------------
// ---------------------------------- Toy3Parser -----------------------------------
// ---------------------------------------------------------------------------------

std::string Toy3Parser::getName()
{
    return "toy-3";
}


void* Toy3Parser::parse(const uint8_t* input, size_t input_size, std::string* error_code)
{
    std::string* toy_output = new std::string;

    *toy_output = std::string((const char*)input, input_size) + " wears a hot hat";

    return (void*)toy_output;
}

differential_parser::ParserOutput* Toy3Parser::normalize
    (void* input, std::string* error_code)
{   
    differential_parser::ParserOutput* returnMe = new
        Toy3ParserOutput((std::string*)input, error_code);
    
    return returnMe;
}
}