#ifndef YAMLCPP_UTILS_H
#define YAMLCPP_UTILS_H

#include <iostream>
#include <fstream>

#include <string>

#include <stack>
#include <map>

#include <assert.h>

#include "include/yaml.h"
#include "yaml-cpp/yaml.h"

std::vector<YAML::Node> parseYamlCpp(const std::string parse_me, std::string* error_message_container);

std::string normalizeYamlCpp(const std::vector<YAML::Node>* nodes, std::string* error_message_container);

std::string normalizeYamlCppNode(const YAML::Node* head, std::string* error_message_container);

#endif