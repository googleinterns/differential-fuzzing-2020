// Copyright 2020 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "toy_fuzz_int_parser.h"

#include <iostream>

namespace toy_int_differential_parser
{
// ---------------------------------------------------------------------------------
// ------------------------------ ToyIntParserOutput ---------------------------------
// ---------------------------------------------------------------------------------

ToyIntParserOutput::ToyIntParserOutput(int* info, std::unique_ptr<std::string> error_code)
{
    this->data = info;

    this->error = std::unique_ptr<std::string>(new std::string());

    this->error = std::move(error_code);
}

ToyIntParserOutput::~ToyIntParserOutput()
{
    if (this->data != nullptr)
    {
        delete this->data;
    }
}

bool ToyIntParserOutput::equivalent(NormalizedOutput* compared_object)
{
    if (!(this->getError()->empty()) && !(compared_object->getError()->empty()))
    {
        if (*this->getError() == *compared_object->getError())
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else if (!(this->getError()->empty() || !(compared_object->getError()->empty())))
    {
        return false;
    }
    return *(int*)this->getData() == *(int*)compared_object->getData();
}

void* ToyIntParserOutput::getData()
{
    return static_cast<void*>(this->data);
}

std::string* ToyIntParserOutput::getError()
{
    return this->error.get();
}

// ---------------------------------------------------------------------------------
// ---------------------------------- ToyIntParser -----------------------------------
// ---------------------------------------------------------------------------------

std::string ToyIntParser::getName()
{
    return "toy-Int";
}


void* ToyIntParser::parse(const uint8_t* input, size_t input_size, std::string* error_code)
{
    int* toy_output = new int;

    *toy_output = (int)(*(const char*)input) + 100;

    return (void*)toy_output;
}

differential_parser::NormalizedOutput* ToyIntParser::normalize(void* input,
    std::unique_ptr<std::string> error_code)
{
    differential_parser::NormalizedOutput* returnMe = new
        ToyIntParserOutput((int*)input, std::move(error_code));
    
    return returnMe;
}
} // namespace toy_int_differential_parser