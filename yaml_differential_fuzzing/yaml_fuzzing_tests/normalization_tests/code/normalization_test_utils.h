#ifndef NORMALIZATION_TEST_UTILS_H
#define NORMALIZATION_TEST_UTILS_H

#include <iostream>
#include <fstream>

#include <string>

#include <stack>
#include <map>

#include <assert.h>

#include "include/yaml.h"
#include "yaml-cpp/yaml.h"

bool compareStringsCustom(const std::string compareMeOne, const std::string compareMeTwo, std::string& buffer);

void PrintNodes(std::vector<YAML::Node>* print_me);

#endif