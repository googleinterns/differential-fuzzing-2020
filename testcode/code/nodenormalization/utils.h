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

enum mode_type {map_type, key_type, value_type, sequence_type, unknown_type};

// ---------------------------------------------------------------------------------
// ------------------------------- libyaml test code -------------------------------
// ---------------------------------------------------------------------------------

bool positionAnalysis(mode_type* add_to_me, const char reference_character, 
    const bool map_mode);

void addToMap(std::map<std::string, std::string>* anchor_map, 
    std::string* anchor, std::string* anchor_data);

bool addToMapDirective(std::map<std::string, std::string>* anchor_map, 
    std::stack<std::string>* anchor_save_stack, std::stack<std::string>* anchor_data_save_stack,
    int& subtract_count, bool interest_in_saving); 

std::string addTag(std::string* tag);

void addToNode(YAML::Node* addToMe, YAML::Node* addMe, std::stack<YAML::Node>* key_stack, 
    const char* tracking_current_type, yaml_char_t* tag);

void end_event_addition
    (std::vector<YAML::Node>* libyaml_final_output, char* mode_stack_top, bool map_mode, 
    std::stack<YAML::Node>* key_stack);

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

#endif