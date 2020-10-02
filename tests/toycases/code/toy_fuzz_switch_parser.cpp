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

#include "toy_fuzz_switch_parser.h"
#include "toy_fuzz_string_output_base.h"

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

    if (*input == '0')
    {
        *toy_output = std::string((const char*)input, input_size) + " wears a hot ";
    }
    else
    {
        *toy_output = std::string((const char*)input, input_size) + " wears a pink ";
    }

    return (void*)toy_output;
}

differential_parser::NormalizedOutput* ToySwitchParser::normalize(void* input, 
    std::unique_ptr<std::string> error_code)
{   
    *(std::string*)input = *(std::string*)input + "pair of glasses";
    differential_parser::NormalizedOutput* returnMe = 
        new toy_generic_string_helper::ToyFuzzGenericStringOutput((std::string*)input, std::move(error_code));

    return returnMe;
}
} // namespace toy_switch_differential_parser