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

#include "libyaml_parser.h"

#include <iostream>

namespace libyaml_differential_parser
{
// ---------------------------------------------------------------------------------
// ---------------------------------- LibyamlParser -----------------------------------
// ---------------------------------------------------------------------------------

std::string LibyamlParser::getName()
{
    return "libyaml";
}


void* LibyamlParser::parse(const uint8_t* input, size_t input_size, std::string* error_code)
{
    std::vector<YAML::Node>* return_me = libyaml_parsing::ParseLibyaml(input, input_size, error_code);
    if (return_me != nullptr)
    {
        return static_cast<void*>(return_me);
    }
    return new std::vector<YAML::Node>();
}

yaml_normalization::YamlNormalizedOutput* LibyamlParser::normalize
    (void* input, std::unique_ptr<std::string> error_code)
{
    if (std::vector<YAML::Node>* casted_input = static_cast<std::vector<YAML::Node>*>(input))
    {
        return new yaml_normalization::YamlNormalizedOutput(static_cast<std::vector<YAML::Node>*>(input), std::move(error_code));
    }
    return new yaml_normalization::YamlNormalizedOutput(new std::vector<YAML::Node>(), std::move(error_code));
}


// ---------------------------------------------------------------------------------
// ---------------------------------- Static Parts ---------------------------------
// ---------------------------------------------------------------------------------

libyaml_differential_parser::LibyamlParser
    libyaml_differential_parser::LibyamlParser::instance;

libyaml_differential_parser::LibyamlParser* LibyamlParser::GetStaticInstance()
{
    return &instance;
}
} // namespace yamlcpp_differential_parser