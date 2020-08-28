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

bool positionAnalysis(mode_type* add_to_me, const mode_type reference_character, 
    const bool map_mode);

std::string addTag(std::string* tag);

void addToNode(YAML::Node* addToMe, YAML::Node* addMe, std::stack<YAML::Node>* key_stack, 
    const mode_type* tracking_current_type, yaml_char_t* tag);

bool endEventAddition
    (std::vector<YAML::Node>* libyaml_final_output, std::stack<mode_type>* mode_stack, 
    std::stack<bool>* map_mode_stack, bool map_mode, std::stack<YAML::Node>* key_stack);
} // namespace

namespace libyaml_parsing
{
std::vector<YAML::Node>& parseLibyaml
        (const uint8_t* input, size_t input_size, std::unique_ptr<std::string>* error_message_container);
} // namespace libyaml_parsing
#endif