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

    // if (compared_object == nullptr) return !this->getError()->empty();

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