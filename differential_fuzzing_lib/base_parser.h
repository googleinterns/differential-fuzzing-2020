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

#ifndef BASE_PARSER_H
#define BASE_PARSER_H

#include <memory>
#include <string>

namespace differential_parser
{
class NormalizedOutput
{
    public:
        virtual ~NormalizedOutput() {};
        virtual bool equivalent(NormalizedOutput* compared_object) = 0;
        virtual void* getData() = 0;
        virtual std::string* getError() = 0;
};

class Parser
{
    public:
        virtual std::string getName() = 0;
        virtual void* parse(const uint8_t* input, size_t input_size, std::string* error_code) = 0;
        virtual differential_parser::NormalizedOutput* normalize(void* input, std::unique_ptr<std::string> error_code) = 0;
};
} // namespace differential_parser

#endif