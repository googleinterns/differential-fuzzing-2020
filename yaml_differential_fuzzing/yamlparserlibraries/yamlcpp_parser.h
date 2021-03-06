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

#ifndef YAMLCPP_PARSER_H
#define YAMLCPP_PARSER_H

#include "base_parser.h"
#include "yaml-cpp/yaml.h"
#include "yaml_normalized_output.h"

#include <string>

namespace yamlcpp_differential_parser
{

// ---------------------------------------------------------------------------------
// ---------------------------------- YamlCppParser -----------------------------------
// ---------------------------------------------------------------------------------
class YamlCppParser : virtual public differential_parser::Parser
{
    private:
        static YamlCppParser instance;
    public:
        std::string getName();
        void* parse(const uint8_t* input, size_t input_size, std::string* error_code);
        yaml_normalization::YamlNormalizedOutput* normalize(void* input, std::unique_ptr<std::string> error_code);
        static YamlCppParser* GetStaticInstance();

};
} // namespace yaml_differential_parser

#endif