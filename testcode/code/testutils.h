#ifndef TESTUTILS_H
#define TESTUTILS_H

#include <iostream>
#include <fstream>

#include <string>

#include <stack>
#include <map>

#include <assert.h>

#include "include/yaml.h"
#include "yaml-cpp/yaml.h"

// ---------------------------------------------------------------------------------
// ------------------------------- libyaml test code -------------------------------
// ---------------------------------------------------------------------------------

bool positionAnalysis(std::string* add_to_me, char reference_character, bool map_mode);

void addToMap(std::map<std::string, std::string>* anchor_map, 
    std::string* anchor, std::string* anchor_data);

bool addToMapDirective(std::map<std::string, std::string>* anchor_map, 
    std::stack<std::string>* anchor_save_stack, std::stack<std::string>* anchor_data_save_stack,
    int& subtract_count, bool interest_in_saving);

bool addToStack(std::stack<std::string>* anchor_save_stack,int* subtract_count, char* anchor);

void addInfoToDataStack(std::stack<std::string>* anchor_data, 
    std::string info);

std::string addTag(std::string* tag);

std::string parseLibyaml(std::string name_of_file, std::string* error_message_container);

// ---------------------------------------------------------------------------------
// ------------------------------ yaml-cpp test code -------------------------------
// ---------------------------------------------------------------------------------

std::string normalizeYamlCppNode(YAML::Node* head, std::string* error_message_container);

// ---------------------------------------------------------------------------------
// ---------------------------------- testcode -------------------------------------
// ---------------------------------------------------------------------------------

bool compareStringsCustom(std::string compareMeOne, std::string compareMeTwo, std::string& buffer);

#endif