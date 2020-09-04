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

namespace
{
enum class mode_type 
{
    MAP_TYPE = 0, 
    KEY_TYPE = 1, 
    VALUE_TYPE = 2, 
    SEQUENCE_TYPE = 3, 
    UNKNOWN_TYPE = 4
};

bool PositionAnalysis(mode_type* add_to_me, const mode_type reference_character, 
    const bool map_mode);

std::string AddTag(std::string* tag);

void AddToNode(YAML::Node* addToMe, YAML::Node* addMe, std::stack<YAML::Node>* key_stack, 
    const mode_type* tracking_current_type, yaml_char_t* tag);

bool EndEventAddition
    (std::vector<YAML::Node>* libyaml_final_output, std::stack<mode_type>* mode_stack, 
    std::stack<bool>* map_mode_stack, bool map_mode, std::stack<YAML::Node>* key_stack);

void RestartVariables (std::stack<YAML::Node>* key_stack,
    std::stack<mode_type>* mode_stack, std::stack<bool>* map_mode_stack,
    std::vector<YAML::Node>* libyaml_local_output, std::vector<YAML::Node>* libyaml_final_output,
    bool* map_mode, std::map<std::string, YAML::Node>* anchor_map);

std::unique_ptr<std::vector<yaml_event_t>> GetEvents
    (const uint8_t* input, size_t input_size, std::string* error_message_container);

void WipeEventList(std::vector<yaml_event_t>* event_list);
} // namespace

namespace libyaml_parsing
{
std::vector<YAML::Node>* ParseLibyaml
        (const uint8_t* input, size_t input_size, std::string* error_message_container);
} // namespace libyaml_parsing
#endif