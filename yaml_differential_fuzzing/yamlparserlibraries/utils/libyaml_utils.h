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

#ifndef LIBYAML_UTILS_H
#define LIBYAML_UTILS_H

#include <iostream>
#include <fstream>

#include <string>

#include <stack>
#include <map>

#include <assert.h>

#include "include/yaml.h"
#include "yaml-cpp/yaml.h"

namespace libyaml_parsing
{
std::vector<YAML::Node>* ParseLibyaml(const uint8_t* input, 
    size_t input_size, std::string* error_message_container);
} // namespace libyaml_parsing
#endif