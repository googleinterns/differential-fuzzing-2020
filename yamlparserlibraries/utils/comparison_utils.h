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

void DisectSequenceNode(YAML::Node* disect_me, std::stack <YAML::Node>* data_save_stack, 
    std::stack <char>* additional_info_stack);

void DisectMapNode(YAML::Node* disect_me, std::stack <YAML::Node>* data_save_stack,
    std::stack <char>* additional_info_stack);

bool CompareSingleNode
    (const YAML::Node* compare_me_one,const YAML::Node* compare_me_two);

bool CompareMultipleNodes
    (const std::vector<YAML::Node>* compare_me_one,const std::vector<YAML::Node>* compare_me_two);

#endif