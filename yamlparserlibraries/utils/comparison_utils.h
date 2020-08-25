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

void DissectSequenceNode(YAML::Node* Dissect_me, std::stack <YAML::Node>* data_save_stack, 
    std::stack <char>* additional_info_stack);

void DissectMapNode(YAML::Node* Dissect_me, std::stack <YAML::Node>* data_save_stack,
    std::stack <char>* additional_info_stack);

bool CompareSingleNode
    (const YAML::Node* compare_me_one,const YAML::Node* compare_me_two);

bool CompareMultipleNodes
    (const std::vector<YAML::Node>* compare_me_one,const std::vector<YAML::Node>* compare_me_two);

bool CompareSingleNodeEmitterBased
    (YAML::Node* compare_me_one,YAML::Node* compare_me_two);

bool CompareMultipleNodesEmitterBased
    (std::vector<YAML::Node>* compare_me_one, std::vector<YAML::Node>* compare_me_two);

#endif