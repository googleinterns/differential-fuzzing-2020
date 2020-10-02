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

#include "yaml_normalized_output.h"
#include "./utils/comparison_utils.h"

// ---------------------------------------------------------------------------------
// ------------------------------ YamlNormalizedOutput --------------------------
// ---------------------------------------------------------------------------------

namespace yaml_normalization
{
YamlNormalizedOutput::YamlNormalizedOutput(std::vector<YAML::Node>* info, std::unique_ptr<std::string> error_code)
{
    this->data = info;

    this->error = std::move(error_code);
}

YamlNormalizedOutput::~YamlNormalizedOutput()
{
    if (this->data != nullptr)
    {
        delete this->data;
    }
}

bool YamlNormalizedOutput::equivalent(NormalizedOutput* compared_object)
{
    std::vector<YAML::Node>* data_one = this->data;

    std::vector<YAML::Node>* data_two = 
        dynamic_cast<yaml_normalization::YamlNormalizedOutput*>(compared_object)->data;

    if (compare_utils::CheckForEmpty(data_one) && compare_utils::CheckForEmpty(data_two))
    {
        return true;
    }

    if (!this->getError()->empty() || !compared_object->getError()->empty())
    {
        return (*this->getError() == *compared_object->getError());
    }
    else
    {
        if (data_one && data_two)
        {
            return compare_utils::CompareMultipleNodes(data_one, data_two);
        }
        else
        {
            return data_one == data_two;
        }
    }
}

void* YamlNormalizedOutput::getData()
{
    return static_cast<void*>(this->data);
}

std::string* YamlNormalizedOutput::getError()
{
    return this->error.get();
}
} // namespace yaml_normalization