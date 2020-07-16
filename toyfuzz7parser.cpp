#include "toyfuzz7parser.h"

#include <iostream>

namespace toy7_differential_parser
{
// ---------------------------------------------------------------------------------
// ------------------------------ Toy7ParserOutput ---------------------------------
// ---------------------------------------------------------------------------------

Toy7ParserOutput::Toy7ParserOutput(int* info, std::string* error_code)
{
    this->data = info;
    this->error = error_code;
}

Toy7ParserOutput::~Toy7ParserOutput()
{
    if (this->data != nullptr)
    {
        delete this->data;
    }
    delete this->error;
}

bool Toy7ParserOutput::equivalent(ParserOutput* compared_object)
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
    return *(int*)this->getData() == *(int*)compared_object->getData();
}

void* Toy7ParserOutput::getData()
{
    return static_cast<void*>(this->data);
}

std::string* Toy7ParserOutput::getError()
{
    return this->error;
}

// ---------------------------------------------------------------------------------
// ---------------------------------- Toy7Parser -----------------------------------
// ---------------------------------------------------------------------------------

std::string Toy7Parser::getName()
{
    return "toy-7";
}


void* Toy7Parser::parse(const uint8_t* input, size_t input_size, std::string* error_code)
{
    int* toy_output = new int;

    *toy_output = (int)(*(const char*)input) + 100;

    return (void*)toy_output;
}

differential_parser::ParserOutput* Toy7Parser::normalize
    (void* input, std::string* error_code)
{   
    differential_parser::ParserOutput* returnMe = new
        Toy7ParserOutput((int*)input, error_code);
    
    return returnMe;
}
}