#include "yaml_normalized_output.h"
#include "./utils/comparison_utils.h"

// ---------------------------------------------------------------------------------
// ------------------------------ YamlNormalizedOutput --------------------------
// ---------------------------------------------------------------------------------

namespace yaml_normalization
{
YamlNormalizedOutput::YamlNormalizedOutput(std::vector<YAML::Node>* info, std::string* error_code)
{
    this->data = info;
    this->error = error_code;
}

YamlNormalizedOutput::~YamlNormalizedOutput()
{
    if (this->data != nullptr)
    {
        delete this->data;
    }
    delete this->error;
}

bool YamlNormalizedOutput::equivalent(NormalizedOutput* compared_object)
{
    if (!(*this->getError()).empty() || !(*compared_object->getError()).empty())
    {
        return (*this->getError() == *compared_object->getError());
    }
    else
    {
        return CompareMultipleNodes
            ((std::vector<YAML::Node>*)this->getData(), (std::vector<YAML::Node>*)compared_object->getData());
    }
}

void* YamlNormalizedOutput::getData()
{
    return static_cast<void*>(this->data);
}

std::string* YamlNormalizedOutput::getError()
{
    return this->error;
}
}