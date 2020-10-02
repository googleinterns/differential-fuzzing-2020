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

#ifndef YAML_NORMALIZED_OUTPUT_H
#define YAML_NORMALIZED_OUTPUT_H

#include "base_parser.h"
#include "yaml-cpp/yaml.h"
namespace yaml_normalization
{
class YamlNormalizedOutput 
    : virtual public differential_parser::NormalizedOutput
{
    public:
        std::vector<YAML::Node>* data;
        std::unique_ptr<std::string> error;

        YamlNormalizedOutput(std::vector<YAML::Node>* info, std::unique_ptr<std::string> error_code);
        ~YamlNormalizedOutput();
        bool equivalent(NormalizedOutput* compared_object);
        void* getData();
        std::string* getError();
};
} // namespace yaml_normalization

#endif