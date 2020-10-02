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

#ifndef TOY_FUZZ_SWITCH_PARSER_H
#define TOY_FUZZ_SWITCH_PARSER_H

#include "base_parser.h"

#include <string>

namespace toy_switch_differential_parser
{
// ---------------------------------------------------------------------------------
// ---------------------------------- ToySwitchParser -----------------------------------
// ---------------------------------------------------------------------------------

class ToySwitchParser : virtual public differential_parser::Parser
{
    
    public:
        std::string getName();
        void* parse(const uint8_t* input, size_t input_size, std::string* error_code);
        differential_parser::NormalizedOutput* normalize(void* input, 
            std::unique_ptr<std::string> error_code);
};
} // namespace toy_switch_differential_parser


#endif