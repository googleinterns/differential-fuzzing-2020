#include "toyfuzz1parser.h"

#include <iostream>

namespace toy1_differential_parser
{
// ---------------------------------------------------------------------------------
// ------------------------------ Toy1ParserOutput ---------------------------------
// ---------------------------------------------------------------------------------

Toy1ParserOutput::Toy1ParserOutput(std::string* info, std::string* error_code)
{
    this->data = info;
    this->error = error_code;
}

Toy1ParserOutput::~Toy1ParserOutput()
{
    if (this->data != nullptr)
    {
        delete this->data;
    }
    delete this->error;
}

bool Toy1ParserOutput::equivalent(ParserOutput* compared_object)
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

void* Toy1ParserOutput::getData()
{
    return static_cast<void*>(this->data);
}

std::string* Toy1ParserOutput::getError()
{
    return this->error;
}

// ---------------------------------------------------------------------------------
// ---------------------------------- Toy1Parser -----------------------------------
// ---------------------------------------------------------------------------------

std::string Toy1Parser::getName()
{
    return "toy-1";
}


void* Toy1Parser::parse(const uint8_t* input, size_t input_size, std::string* error_code)
{
    std::string* toy_output = new std::string;

    *toy_output = std::string((const char*)input, input_size) + " wears a cool hat";

    return (void*)toy_output;
}

differential_parser::ParserOutput* Toy1Parser::normalize
    (void* input, std::string* error_code)
{   
    differential_parser::ParserOutput* returnMe = new
        Toy1ParserOutput((std::string*)input, error_code);
    
    return returnMe;
}
}