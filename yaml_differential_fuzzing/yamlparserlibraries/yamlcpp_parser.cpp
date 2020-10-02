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

#include "yamlcpp_parser.h"

#include "./utils/comparison_utils.h"

namespace yamlcpp_differential_parser
{
// ---------------------------------------------------------------------------------
// ---------------------------------- YamlCppParser --------------------------------
// ---------------------------------------------------------------------------------

std::string YamlCppParser::getName()
{
    return "yaml-cpp";
}

void* YamlCppParser::parse(const uint8_t* input, size_t input_size, std::string* error_code)
{
    std::vector<YAML::Node>* yaml_cpp_loop_temp = new std::vector<YAML::Node>;

    try
    {
        if (input != nullptr)
        {
            *yaml_cpp_loop_temp = YAML::LoadAll(std::string((const char*)input, input_size));
        }
        else
        {
            *error_code = std::string("ERROR");
            return nullptr;
        }
    }
    catch (const std::exception& err)
    {
        *error_code = std::string("ERROR");
    }

    return static_cast<void*>(yaml_cpp_loop_temp);
}

yaml_normalization::YamlNormalizedOutput* YamlCppParser::normalize
    (void* input, std::unique_ptr<std::string> error_code)
{
    if (std::vector<YAML::Node>* casted_input = static_cast<std::vector<YAML::Node>*>(input))
    {
        return new
            yaml_normalization::YamlNormalizedOutput(casted_input, std::move(error_code));
    }

    return nullptr;
}

// ---------------------------------------------------------------------------------
// ---------------------------------- Static Parts ---------------------------------
// ---------------------------------------------------------------------------------

yamlcpp_differential_parser::YamlCppParser 
    yamlcpp_differential_parser::YamlCppParser::instance;

yamlcpp_differential_parser::YamlCppParser* YamlCppParser::GetStaticInstance()
{
    return &instance;
}
} // namespace yamlcpp_differential_parser