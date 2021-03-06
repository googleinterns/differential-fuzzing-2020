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

#include "toy_fuzz_string_output_base.h"

#include <iostream>

namespace toy_generic_string_helper
{
// ---------------------------------------------------------------------------------
// ------------------------- ToyFuzzGenericStringOutput ----------------------------
// ---------------------------------------------------------------------------------

ToyFuzzGenericStringOutput::ToyFuzzGenericStringOutput(std::string* info, std::unique_ptr<std::string> error_code)
{
    this->data = info;

    this->error = std::move(error_code);
}

ToyFuzzGenericStringOutput::~ToyFuzzGenericStringOutput()
{
    if (this->data != nullptr)
    {
        delete this->data;
    }
}

bool ToyFuzzGenericStringOutput::equivalent(NormalizedOutput* compared_object)
{
    return toy_generic_string_helper::compareStrings(this, compared_object);
}

void* ToyFuzzGenericStringOutput::getData()
{
    return static_cast<void*>(this->data);
}

std::string* ToyFuzzGenericStringOutput::getError()
{
    return this->error.get();
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

std::string ToyFuzzGenericStringParser::getName()
{
    return this->name_modifier;
}


void* ToyFuzzGenericStringParser::parse(const uint8_t* input, size_t input_size, std::string* error_code)
{
    std::string* toy_output = new std::string();

    *toy_output = std::string((const char*)input, input_size) + this->parser_modifier;

    *error_code = this->error_modifier;

    return (void*)toy_output;
}

differential_parser::NormalizedOutput* ToyFuzzGenericStringParser::normalize
    (void* input,  std::unique_ptr<std::string> error_code)
{
    *(std::string*)input = *(std::string*)input + this->normalizer_modifier;
    differential_parser::NormalizedOutput* returnMe = 
        new toy_generic_string_helper::ToyFuzzGenericStringOutput((std::string*)input, std::move(error_code));
    
    return returnMe;
}

// ---------------------------------------------------------------------------------
// ---------------------- Helper Compare Strings Method ----------------------------
// ---------------------------------------------------------------------------------

bool compareStrings(differential_parser::NormalizedOutput* compared_object_one, 
    differential_parser::NormalizedOutput* compared_object_two)
{
    if (!compared_object_two->getError()->empty() && !(compared_object_one->getError()->empty()))
    {
        if ((*compared_object_two->getError()) == (*compared_object_one->getError()))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else if (!compared_object_two->getError()->empty()
        || !compared_object_one->getError()->empty())
    {
        return false;
    }
    return *(std::string*)compared_object_two->getData() == *(std::string*)compared_object_one->getData();
}
} // namespace toy_generic_string_helper