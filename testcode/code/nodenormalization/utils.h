#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <fstream>

#include <string>

#include <stack>
#include <map>

#include <assert.h>

#include "include/yaml.h"
#include "yaml-cpp/yaml.h"

enum class mode_type 
{
    MAP_TYPE = 0, 
    KEY_TYPE = 1, 
    VALUE_TYPE = 2, 
    SEQUENCE_TYPE = 3, 
    UNKNOWN_TYPE = 4
};

// ---------------------------------------------------------------------------------
// ------------------------------- libyaml test code -------------------------------
// ---------------------------------------------------------------------------------

bool positionAnalysis(mode_type* add_to_me, const mode_type reference_character, 
    const bool map_mode);

std::string addTag(std::string* tag);

void addToNode(YAML::Node* addToMe, YAML::Node* addMe, std::stack<YAML::Node>* key_stack, 
    const mode_type* tracking_current_type, yaml_char_t* tag);

bool endEventAddition
    (std::vector<YAML::Node>* libyaml_final_output, std::stack<mode_type>* mode_stack, 
    std::stack<bool>* map_mode_stack, bool map_mode, std::stack<YAML::Node>* key_stack);

std::string parseLibyaml(const std::string name_of_file, std::string* error_message_container);

std::vector<YAML::Node> normalizeLibyaml
        (std::string name_of_file, std::string* error_message_container);
// ---------------------------------------------------------------------------------
// ------------------------------ yaml-cpp test code -------------------------------
// ---------------------------------------------------------------------------------

std::vector<YAML::Node> parseYamlCpp(const std::string parse_me, std::string* error_message_container);

std::string normalizeYamlCpp(const std::vector<YAML::Node>* nodes, std::string* error_message_container);

std::string normalizeYamlCppNode(const YAML::Node* head, std::string* error_message_container);

// ---------------------------------------------------------------------------------
// ---------------------------------- testcode -------------------------------------
// ---------------------------------------------------------------------------------

bool compareStringsCustom(const std::string compareMeOne, const std::string compareMeTwo, std::string& buffer);

// ---------------------------------------------------------------------------------
// --------------------------- node based comparison -------------------------------
// ---------------------------------------------------------------------------------

void disectSequenceNode(YAML::Node* disect_me, std::stack <YAML::Node>* data_save_stack);

void disectMapNode(YAML::Node* disect_me, std::stack <YAML::Node>* data_save_stack);

bool compareSingleNode
    (const YAML::Node* compare_me_one,const YAML::Node* compare_me_two);

bool compareMultipleNodes
    (const std::vector<YAML::Node>* compare_me_one,const std::vector<YAML::Node>* compare_me_two);

#endif