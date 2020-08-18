#ifndef COMPARISON_UTILS_H
#define COMPARISON_UTILS_H

#include <iostream>
#include <fstream>

#include <string>

#include <stack>
#include <map>

#include <assert.h>

#include "include/yaml.h"
#include "yaml-cpp/yaml.h"

void disectSequenceNode(YAML::Node* disect_me, std::stack <YAML::Node>* data_save_stack);

void disectMapNode(YAML::Node* disect_me, std::stack <YAML::Node>* data_save_stack);

bool compareSingleNode
    (const YAML::Node* compare_me_one,const YAML::Node* compare_me_two);

bool compareMultipleNodes
    (const std::vector<YAML::Node>* compare_me_one,const std::vector<YAML::Node>* compare_me_two);

#endif