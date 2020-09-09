#include "comparison_utils.h"

namespace
{
static bool CompareSingleNode
    (YAML::Node* compare_me_one,YAML::Node* compare_me_two)
{
    std::stringstream stream_node_one;
    std::stringstream stream_node_two;
    compare_me_one->SetStyle(YAML::EmitterStyle::Flow);
    compare_me_two->SetStyle(YAML::EmitterStyle::Flow);
    if(compare_me_one==nullptr)
    {
        std::cerr << "Howeven" << std::endl;
    }

        if(compare_me_two==nullptr)
    {
        std::cerr << "Howeven2" << std::endl;
    }
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

bool CompareMultipleNodes
    (std::vector<YAML::Node>* compare_me_one, std::vector<YAML::Node>* compare_me_two)
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