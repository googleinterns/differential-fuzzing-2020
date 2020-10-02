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

#include "comparison_utils.h"

namespace
{
static bool CompareSingleNode(YAML::Node* compare_me_one,
    YAML::Node* compare_me_two)
{
    std::stringstream stream_node_one;
    std::stringstream stream_node_two;

    compare_me_one->SetStyle(YAML::EmitterStyle::Flow);
    compare_me_two->SetStyle(YAML::EmitterStyle::Flow);

    stream_node_one << *compare_me_one;
    stream_node_two << *compare_me_two;

    return stream_node_one.str() == stream_node_two.str();

}
} // namespace

namespace compare_utils
{
// ---------------------------------------------------------------------------------
// --------------------------- emitter based comparison ----------------------------
// ---------------------------------------------------------------------------------

bool CheckForEmpty(std::vector<YAML::Node>*  compare_me)
{
    if (compare_me == nullptr) return true;
    return compare_me->empty();
}

bool CompareMultipleNodes(std::vector<YAML::Node>* compare_me_one, 
    std::vector<YAML::Node>* compare_me_two)
{
    std::vector<YAML::Node>::iterator iterator_one = compare_me_one->begin();
    std::vector<YAML::Node>::iterator iterator_two = compare_me_two->begin();

    while (iterator_one != compare_me_one->end() && iterator_two != compare_me_two->end() &&
        CompareSingleNode(&(*iterator_one), &(*iterator_two)))
    {
        iterator_one++;
        iterator_two++;
    }

    return (iterator_one == compare_me_one->end() && iterator_two == compare_me_two->end());
}

} // namespace compare_utils