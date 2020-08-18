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

bool compareStringsCustom(const std::string compareMeOne, const std::string compareMeTwo, std::string& buffer);

#endif