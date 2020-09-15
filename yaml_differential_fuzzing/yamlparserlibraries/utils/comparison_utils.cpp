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

    // if (compare_me->size() == 1)
    // {
    //     return !compare_me->back().IsDefined();
    // }
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