#ifndef COMPARISON_UTILS_H
#define COMPARISON_UTILS_H

#include <iostream>
#include <fstream>

#include <string>
#include <sstream>

#include <stack>
#include <map>

#include <assert.h>

#include "include/yaml.h"
#include "yaml-cpp/yaml.h"

bool CompareMultipleNodes
    (const std::vector<YAML::Node>* compare_me_one,const std::vector<YAML::Node>* compare_me_two);

bool CompareMultipleNodesEmitterBased
    (std::vector<YAML::Node>* compare_me_one, std::vector<YAML::Node>* compare_me_two);

#endif