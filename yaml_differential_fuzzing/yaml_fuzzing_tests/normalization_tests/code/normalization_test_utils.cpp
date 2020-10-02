// Copyright 2020 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "normalization_test_utils.h"

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>


bool compareBufferCustom(const std::string compare_me_one, 
    const std::string compare_me_two, std::string& buffer)
{
    std::string::const_iterator ptrOne = compare_me_one.begin();
    std::string::const_iterator ptrTwo = compare_me_two.begin();

    while (*ptrOne == *ptrTwo && ((ptrOne != compare_me_one.end()) || (ptrTwo != compare_me_two.end())))
    {
        if(*ptrOne == *ptrTwo)
        {
            buffer += *ptrTwo;
        }
        ptrOne++;
        ptrTwo++;
    }

    if (!((ptrOne == compare_me_one.end())  && (ptrTwo == compare_me_two.end())))
    {
        buffer += "(X)";
        return false;
    }
    else
    {
        return true;
    }
}

void PrintNodes(std::vector<YAML::Node>* print_me)
{
    for (std::vector<YAML::Node>::iterator iterator = print_me->begin(); 
        iterator != print_me->end(); iterator++)
    {
        std::stringstream stream_node;

        iterator->SetStyle(YAML::EmitterStyle::Block);

        std::cout << *iterator << std::endl;
    }
}