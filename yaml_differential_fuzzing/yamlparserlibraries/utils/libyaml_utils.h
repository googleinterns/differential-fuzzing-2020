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