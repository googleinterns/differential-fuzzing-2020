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

        YamlNormalizedOutput(std::vector<YAML::Node>* info, std::unique_ptr<std::string>* error_code);
        ~YamlNormalizedOutput();
        bool equivalent(NormalizedOutput* compared_object);
        void* getData();
        std::unique_ptr<std::string>* getError();
};
} // namespace yaml_normalization

#endif