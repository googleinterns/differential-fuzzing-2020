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

#ifndef TOY_FUZZ_INT_PARSER_H
#define TOY_FUZZ_INT_PARSER_H

#include "base_parser.h"

#include <string>

namespace toy_int_differential_parser
{
// ---------------------------------------------------------------------------------
// ------------------------------ ToyIntParserOutput ---------------------------------
// ---------------------------------------------------------------------------------

class ToyIntParserOutput 
    : virtual public differential_parser::NormalizedOutput
{
    private:
        int* data;
        std::unique_ptr<std::string> error;
    public:
        ToyIntParserOutput(int* info, std::unique_ptr<std::string> error_code);
        ~ToyIntParserOutput();
        bool equivalent(NormalizedOutput* compared_object);
        void* getData();
        std::string* getError();
};

// ---------------------------------------------------------------------------------
// ---------------------------------- ToyIntParser -----------------------------------
// ---------------------------------------------------------------------------------

class ToyIntParser : virtual public differential_parser::Parser
{
    public:
        std::string getName();
        void* parse(const uint8_t* input, size_t input_size, std::string* error_code);
        differential_parser::NormalizedOutput* normalize(void* input, 
            std::unique_ptr<std::string> error_code);
};
} // namespace toy_int_differential_parser


#endif