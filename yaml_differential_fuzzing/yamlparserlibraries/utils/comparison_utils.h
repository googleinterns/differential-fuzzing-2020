#ifndef COMPARISON_UTILS_H
#define COMPARISON_UTILS_H

#include <iostream>
#include <fstream>

#include <string>
#include <sstream>

#include "include/yaml.h"
#include "yaml-cpp/yaml.h"

namespace compare_utils
{
bool CompareMultipleNodes(std::vector<YAML::Node>* compare_me_one, 
    std::vector<YAML::Node>* compare_me_two);

bool CheckForEmpty(std::vector<YAML::Node>*  compare_me);
}

#endif