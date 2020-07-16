#include "toyfuzz8parser.h"

#include <iostream>

namespace toy8_differential_parser
{
// ---------------------------------------------------------------------------------
// ------------------------------ Toy8ParserOutput ---------------------------------
// ---------------------------------------------------------------------------------

Toy8ParserOutput::Toy8ParserOutput(std::string* info, std::string* error_code)
{
    this->data = info;
    this->error = error_code;
}

Toy8ParserOutput::~Toy8ParserOutput()
{
    if (this->data != nullptr)
    {
        delete this->data;
    }
    delete this->error;
}

bool Toy8ParserOutput::equivalent(ParserOutput* compared_object)
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

void* Toy8ParserOutput::getData()
{
    return static_cast<void*>(this->data);
}

std::string* Toy8ParserOutput::getError()
{
    return this->error;
}

// ---------------------------------------------------------------------------------
// ---------------------------------- Toy8Parser -----------------------------------
// ---------------------------------------------------------------------------------

std::string Toy8Parser::getName()
{
    return "toy-8";
}


void* Toy8Parser::parse(const uint8_t* input, size_t input_size, std::string* error_code)
{
    std::string* toy_output = new std::string;

    *toy_output = std::string((const char*)input, input_size) + " wears a hot ";

    return (void*)toy_output;
}

differential_parser::ParserOutput* Toy8Parser::normalize
    (void* input, std::string* error_code)
{   
    *(std::string*)input = *(std::string*)input + "pair of glasses";
    differential_parser::ParserOutput* returnMe = new
        Toy8ParserOutput((std::string*)input, error_code);
    
    return returnMe;
}
}